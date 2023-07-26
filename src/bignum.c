#include "conf.h"
#include <myy/bignum.h>
#include <myy/endian.h>

#define __ADD_ASM_UNIT_1__(x) \
		"movq "#x"(%1), %%rax\n\t"\
		"adcq "#x"(%2), %%rax\n\t"\
		"movq %%rax, "#x"(%0)\n\t"
void bn_256_add(BN_256 r, const BN_256 a, const BN_256 b){
	asm volatile (
		"movq 24(%1), %%rax\n\t"
		"addq 24(%2), %%rax\n\t"
		"movq %%rax, 24(%0)\n\t"
		__ADD_ASM_UNIT_1__(16)
		__ADD_ASM_UNIT_1__(8)
		__ADD_ASM_UNIT_1__()
		::"r"(r),"r"(a),"r"(b)
		:"rax"
	);
}
#undef __ADD_ASM_UNIT_1__

#define __ADC_ASM_UNIT_1__(x) \
		"movq "#x"(%2), %%rax\n\t"\
		"adcq "#x"(%3), %%rax\n\t"\
		"movq %%rax, "#x"(%1)\n\t"
int bn_256_adc(BN_256 r, const BN_256 a, const BN_256 b){
	int f=0;
	asm volatile (
		"movq 24(%2), %%rax\n\t"
		"addq 24(%3), %%rax\n\t"
		"movq %%rax, 24(%1)\n\t"
		__ADC_ASM_UNIT_1__(16)
		__ADC_ASM_UNIT_1__(8)
		__ADC_ASM_UNIT_1__()
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(r),"r"(a),"r"(b)
		:"rax"
	);
	return f;
}

#define __INC_ASM_UNIT_1__(x) \
		"movq "#x"(%1), %%rax\n\t"\
		"adcq %%rax, "#x"(%0)\n\t"
void bn_256_inc(BN_256 a, const BN_256 b){
	asm volatile(
		"movq 24(%1), %%rax\n\t"
		"addq %%rax, 24(%0)\n\t"
		__INC_ASM_UNIT_1__(16)
		__INC_ASM_UNIT_1__(8)
		__INC_ASM_UNIT_1__()
		::"r"(a),"r"(b)
		:"rax"
	);
}
#undef __INC_ASM_UNIT_1__

int bn_256_inc1(BN_256 a){
	int f=0;
	asm(
		"addq $1, 24(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addq $1, 16(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addq $1, 8(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addq $1, 0(%1)\n\t"
		"__bn_256_inc1_exit:\n\t"
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(a)
		:"rax"
	);
	return f;
}

void bn_256_sub(BN_256 r, const BN_256 a, const BN_256 b){
	asm volatile (
		"movq 24(%1), %%rax\n\t"
		"subq 24(%2), %%rax\n\t"
		"movq %%rax, 24(%0)\n\t"
		"movq 16(%1), %%rax\n\t"
		"sbbq 16(%2), %%rax\n\t"
		"movq %%rax, 16(%0)\n\t"
		"movq 8(%1), %%rax\n\t"
		"sbbq 8(%2), %%rax\n\t"
		"movq %%rax, 8(%0)\n\t"
		"movq (%1), %%rax\n\t"
		"sbbq (%2), %%rax\n\t"
		"movq %%rax, (%0)\n\t"
		::"r"(r),"r"(a),"r"(b)
		:"rax"
	);
}

int bn_256_cmp(const BN_256 a, const BN_256 b){
	for (int i=0;i<4;i++){
		if(a[i]>b[i]) return 1;
		if(a[i]<b[i]) return -1;
	}
	return 0;
}

void bn_256_set_zero(BN_256 bn){
	bn[0] = 0; bn[1] = 0;
	bn[2] = 0; bn[3] = 0;
}

void bn_256_set_word(BN_256 bn, uint32_t w){
	bn[0] = 0; bn[1] = 0;
	bn[2] = 0; bn[3] = w;
}

void bn_256_to_bin(BN_256 bn, uint8_t* dest){
	((BN_UNIT*)dest)[0]=h2be_64(bn[0]);
	((BN_UNIT*)dest)[1]=h2be_64(bn[1]);
	((BN_UNIT*)dest)[2]=h2be_64(bn[2]);
	((BN_UNIT*)dest)[3]=h2be_64(bn[3]);
}

void bn_256_from_bin(BN_256 bn, uint8_t* src){
	bn[0]=be2h_64(((BN_UNIT*)src)[0]);
	bn[1]=be2h_64(((BN_UNIT*)src)[1]);
	bn[2]=be2h_64(((BN_UNIT*)src)[2]);
	bn[3]=be2h_64(((BN_UNIT*)src)[3]);
}

#define __MUL_ASM_UNIT_1__(a,x,t) \
		"movq "#a"(%1), %%rax\n\t"\
		"mulq "#x"(%2)\n\t"\
		"addq %%rbx, %%rax\n\t"\
		"movq %%rax, "#t"(%3)\n\t"\
		"adcq $0, %%rdx\n\t"\
		"movq %%rdx, %%rbx\n\t"\

#define __MUL_ASM_UNIT_2__(x)		 \
		"movq 24(%1), %%rax\n\t"\
		"mulq "#x"(%2)\n\t"\
		"movq %%rax, 32(%3)\n\t"\
		"movq %%rdx, %%rbx\n\t"\
		__MUL_ASM_UNIT_1__(16,x,24)\
		__MUL_ASM_UNIT_1__(8,x,16)\
		"movq (%1), %%rax\n\t"\
		"mulq "#x"(%2)\n\t"\
		"addq %%rbx, %%rax\n\t"\
		"movq %%rax, 8(%3)\n\t"\
		"adcq $0, %%rdx\n\t"\
		"movq %%rdx, (%3)\n\t"

#define __MUL_ASM_UNIT_3__(a,b,c,d,e) \
		"movq 32(%3), %%rax\n\t"\
		"addq %%rax, "#a"(%0)\n\t"\
		"movq 24(%3), %%rax\n\t"\
		"adcq %%rax, "#b"(%0)\n\t"\
		"movq 16(%3), %%rax\n\t"\
		"adcq %%rax, "#c"(%0)\n\t"\
		"movq 8(%3), %%rax\n\t"\
		"adcq %%rax, "#d"(%0)\n\t"\
		"movq (%3), %%rax\n\t"\
		"adcq %%rax, "#e"(%0)\n\t"

void bn_256_mul(BN_512 r, const BN_256 a, const BN_256 b){
	BN_UNIT t[5];
	r[0]=0;r[1]=0;r[2]=0;
	//r[3]=0;
	//r[4]=0;r[5]=0;r[6]=0;r[7]=0;
	asm volatile (
		"movq 24(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"movq %%rax, 56(%0)\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq 16(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, 48(%0)\n\t"
		"adcq $0, %%rdx\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq 8(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, 40(%0)\n\t"
		"adcq $0, %%rdx\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq (%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, 32(%0)\n\t"
		"adcq $0, %%rdx\n\t"
		"movq %%rdx, 24(%0)\n\t"
		__MUL_ASM_UNIT_2__(16)
		__MUL_ASM_UNIT_3__(48,40,32,24,16)
		__MUL_ASM_UNIT_2__(8)
		__MUL_ASM_UNIT_3__(40,32,24,16,8)
		__MUL_ASM_UNIT_2__()
		__MUL_ASM_UNIT_3__(32,24,16,8,)
		::"r"(r),"r"(a),"r"(b),"r"(t)
		:"rax","rbx","rdx"
	);
}
/*
    |ABCD
    |abcd
----|-------
   J|ABCD
  JA|BCD
 JAB|CD
JABC|D

*/
void bn_256_imul(BN_256 r, const BN_256 a, const BN_256 b){
	BN_UNIT t[3];
	asm volatile(
		"movq 24(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"movq %%rax, 24(%0)\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq 16(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, 16(%0)\n\t"
		"adcq $0, %%rdx\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq 8(%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, 8(%0)\n\t"
		"adcq $0, %%rdx\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq (%1), %%rax\n\t"
		"mulq 24(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, (%0)\n\t"
//===============================
		"movq 24(%1), %%rax\n\t"
		"mulq 16(%2)\n\t"
		"movq %%rax, 16(%3)\n\t"
		"movq %%rdx, %%rbx\n\t"
		__MUL_ASM_UNIT_1__(16,16,8)
		"movq 8(%1), %%rax\n\t"
		"mulq 16(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, (%3)\n\t"

		"movq 16(%3), %%rax\n\t"
		"addq %%rax, 16(%0)\n\t"
		"movq 8(%3), %%rax\n\t"
		"adcq %%rax, 8(%0)\n\t"
		"movq 0(%3), %%rax\n\t"
		"adcq %%rax, (%0)\n\t"
//===============================
		"movq 24(%1), %%rax\n\t"
		"mulq 8(%2)\n\t"
		"movq %%rax, 8(%3)\n\t"
		"movq %%rdx, %%rbx\n\t"

		"movq 16(%1), %%rax\n\t"
		"mulq 8(%2)\n\t"
		"addq %%rbx, %%rax\n\t"
		"movq %%rax, (%3)\n\t"

		"movq 8(%3), %%rax\n\t"
		"addq %%rax, 8(%0)\n\t"
		"movq 0(%3), %%rax\n\t"
		"adcq %%rax, (%0)\n\t"
//===============================
		"movq 24(%1), %%rax\n\t"
		"mulq (%2)\n\t"
		"addq %%rax, (%0)\n\t"
		::"r"(r),"r"(a),"r"(b),"r"(t)
		:"rax","rbx","rdx"
	);
}

void bn_256_cpy(BN_256 r, const BN_256 a){
	r[0]=a[0];r[1]=a[1];
	r[2]=a[2];r[3]=a[3];
}

#undef __MUL_ASM_UNIT_1__
#undef __MUL_ASM_UNIT_2__
#undef __MUL_ASM_UNIT_3__

/* r=a-b，取512位数ab的低288位，将其相减 */
void __bn_288_sub(BN_512 r, const BN_512 a, const BN_512 b){
	asm volatile (
		"movq 56(%1), %%rax\n\t"
		"subq 56(%2), %%rax\n\t"
		"movq %%rax, 56(%0)\n\t"
		"movq 48(%1), %%rax\n\t"
		"sbbq 48(%2), %%rax\n\t"
		"movq %%rax, 48(%0)\n\t"
		"movq 40(%1), %%rax\n\t"
		"sbbq 40(%2), %%rax\n\t"
		"movq %%rax, 40(%0)\n\t"
		"movq 32(%1), %%rax\n\t"
		"sbbq 32(%2), %%rax\n\t"
		"movq %%rax, 32(%0)\n\t"
		"movq 24(%1), %%rax\n\t"
		"sbbq 24(%2), %%rax\n\t"
		"movq %%rax, 24(%0)\n\t"
		::"r"(r),"r"(a),"r"(b)
		:"rax"
	);
}

/* r=r-b，取512的低288位，将其减去一个256位 */
void __bn_288_sbb_256(BN_512 r, const BN_256 b){
	asm volatile(
		"movq 24(%1), %%rax\n\t"
		"subq %%rax, 56(%0)\n\t"
		"movq 16(%1), %%rax\n\t"
		"sbbq %%rax, 48(%0)\n\t"
		"movq 8(%1), %%rax\n\t"
		"sbbq %%rax, 40(%0)\n\t"
		"movq (%1), %%rax\n\t"
		"sbbq %%rax, 32(%0)\n\t"
		"sbbq $0, 24(%0)\n\t"
		::"r"(r),"r"(b)
		:"rax"
	);
}

/* 将一个512位的低288位与一个256位比较，256位数视为前方补零的288位数 */
int __bn_288_cmp_256(const BN_512 a, const BN_256 b){
	if(a[3]>0){
		return 1;
	}
	return bn_256_cmp(bn_512_l256(a),b);
}

/* 将两个512位的高256位相加，还要加上一位进位，因为已知a+b的结果低256位为0 */
/* 但如此一来需要确定b的低256位不为0,否则没有进位 */
int __bn_512_h256_adc(BN_256 r, const BN_512 a, const BN_512 b){
	int f=0;
	asm volatile(
		"stc\n\t"
		__ADC_ASM_UNIT_1__(24)
		__ADC_ASM_UNIT_1__(16)
		__ADC_ASM_UNIT_1__(8)
		__ADC_ASM_UNIT_1__()
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(r),"r"(a),"r"(b)
		:"rax"
	);
	return f;
}
#undef __ADC_ASM_UNIT_1__
