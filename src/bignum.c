#include "conf.h"
#include <myy/bignum.h>
#include <myy/endian.h>

#define __ADD_ASM_UNIT_1__(x) \
		"movl "#x"(%1), %%eax\n\t"\
		"adcl "#x"(%2), %%eax\n\t"\
		"movl %%eax, "#x"(%0)\n\t"
void bn_256_add(BN_256 r, const BN_256 a, const BN_256 b){
	asm volatile (
		"movl 28(%1), %%eax\n\t"
		"addl 28(%2), %%eax\n\t"
		"movl %%eax, 28(%0)\n\t"
		__ADD_ASM_UNIT_1__(24)
		__ADD_ASM_UNIT_1__(20)
		__ADD_ASM_UNIT_1__(16)
		__ADD_ASM_UNIT_1__(12)
		__ADD_ASM_UNIT_1__(8)
		__ADD_ASM_UNIT_1__(4)
		__ADD_ASM_UNIT_1__()
		::"r"(r),"r"(a),"r"(b)
		:"eax"
	);
}
#undef __ADD_ASM_UNIT_1__

#define __ADC_ASM_UNIT_1__(x) \
		"movl "#x"(%2), %%eax\n\t"\
		"adcl "#x"(%3), %%eax\n\t"\
		"movl %%eax, "#x"(%1)\n\t"
int bn_256_adc(BN_256 r, const BN_256 a, const BN_256 b){
	int f=0;
	asm volatile (
		"movl 28(%2), %%eax\n\t"
		"addl 28(%3), %%eax\n\t"
		"movl %%eax, 28(%1)\n\t"
		__ADC_ASM_UNIT_1__(24)
		__ADC_ASM_UNIT_1__(20)
		__ADC_ASM_UNIT_1__(16)
		__ADC_ASM_UNIT_1__(12)
		__ADC_ASM_UNIT_1__(8)
		__ADC_ASM_UNIT_1__(4)
		__ADC_ASM_UNIT_1__()
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(r),"r"(a),"r"(b)
		:"eax"
	);
	return f;
}

#define __INC_ASM_UNIT_1__(x) \
		"movl "#x"(%1), %%eax\n\t"\
		"adcl %%eax, "#x"(%0)\n\t"
void bn_256_inc(BN_256 a, const BN_256 b){
	asm volatile(
		"movl 28(%1), %%eax\n\t"
		"addl %%eax, 28(%0)\n\t"
		__INC_ASM_UNIT_1__(24)
		__INC_ASM_UNIT_1__(20)
		__INC_ASM_UNIT_1__(16)
		__INC_ASM_UNIT_1__(12)
		__INC_ASM_UNIT_1__(8)
		__INC_ASM_UNIT_1__(4)
		__INC_ASM_UNIT_1__()
		::"r"(a),"r"(b)
		:"eax"
	);
}
#undef __INC_ASM_UNIT_1__

int bn_256_inc1(BN_256 a){
	int f=0;
	asm(
		"addl $1, 28(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 24(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 20(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 16(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 12(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 8(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 4(%1)\n\t"
		"JNC __bn_256_inc1_exit\n\t"
		"addl $1, 0(%1)\n\t"
		"__bn_256_inc1_exit:\n\t"
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(a)
		:"eax"
	);
	return f;
}

void bn_256_sub(BN_256 r, const BN_256 a, const BN_256 b){
	asm volatile (
		"movl 28(%1), %%eax\n\t"
		"subl 28(%2), %%eax\n\t"
		"movl %%eax, 28(%0)\n\t"
		"movl 24(%1), %%eax\n\t"
		"sbbl 24(%2), %%eax\n\t"
		"movl %%eax, 24(%0)\n\t"
		"movl 20(%1), %%eax\n\t"
		"sbbl 20(%2), %%eax\n\t"
		"movl %%eax, 20(%0)\n\t"
		"movl 16(%1), %%eax\n\t"
		"sbbl 16(%2), %%eax\n\t"
		"movl %%eax, 16(%0)\n\t"
		"movl 12(%1), %%eax\n\t"
		"sbbl 12(%2), %%eax\n\t"
		"movl %%eax, 12(%0)\n\t"
		"movl 8(%1), %%eax\n\t"
		"sbbl 8(%2), %%eax\n\t"
		"movl %%eax, 8(%0)\n\t"
		"movl 4(%1), %%eax\n\t"
		"sbbl 4(%2), %%eax\n\t"
		"movl %%eax, 4(%0)\n\t"
		"movl (%1), %%eax\n\t"
		"sbbl (%2), %%eax\n\t"
		"movl %%eax, (%0)\n\t"
		::"r"(r),"r"(a),"r"(b)
		:"eax"
	);
}

int bn_256_cmp(const BN_256 a, const BN_256 b){
	for (int i=0;i<8;i++){
		if(a[i]>b[i]) return 1;
		if(a[i]<b[i]) return -1;
	}
	return 0;
}

void bn_256_set_zero(BN_256 bn){
	bn[0] = 0; bn[1] = 0;
	bn[2] = 0; bn[3] = 0;
	bn[4] = 0; bn[5] = 0;
	bn[6] = 0; bn[7] = 0;
}

void bn_256_set_word(BN_256 bn, uint32_t w){
	bn[0] = 0; bn[1] = 0;
	bn[2] = 0; bn[3] = 0;
	bn[4] = 0; bn[5] = 0;
	bn[6] = 0; bn[7] = w;
}

void bn_256_to_bin(BN_256 bn, uint8_t* dest){
	((uint32_t*)dest)[0]=h2be_32(bn[0]);
	((uint32_t*)dest)[1]=h2be_32(bn[1]);
	((uint32_t*)dest)[2]=h2be_32(bn[2]);
	((uint32_t*)dest)[3]=h2be_32(bn[3]);
	((uint32_t*)dest)[4]=h2be_32(bn[4]);
	((uint32_t*)dest)[5]=h2be_32(bn[5]);
	((uint32_t*)dest)[6]=h2be_32(bn[6]);
	((uint32_t*)dest)[7]=h2be_32(bn[7]);
}

void bn_256_from_bin(BN_256 bn, uint8_t* src){
	bn[0]=be2h_32(((uint32_t*)src)[0]);
	bn[1]=be2h_32(((uint32_t*)src)[1]);
	bn[2]=be2h_32(((uint32_t*)src)[2]);
	bn[3]=be2h_32(((uint32_t*)src)[3]);
	bn[4]=be2h_32(((uint32_t*)src)[4]);
	bn[5]=be2h_32(((uint32_t*)src)[5]);
	bn[6]=be2h_32(((uint32_t*)src)[6]);
	bn[7]=be2h_32(((uint32_t*)src)[7]);
}

#define __MUL_ASM_UNIT_1__(a,x,t) \
		"movl "#a"(%1), %%eax\n\t"\
		"mull "#x"(%2)\n\t"\
		"addl %%ebx, %%eax\n\t"\
		"movl %%eax, "#t"(%3)\n\t"\
		"adcl $0, %%edx\n\t"\
		"movl %%edx, %%ebx\n\t"\

#define __MUL_ASM_UNIT_2__(x)		 \
		"movl 28(%1), %%eax\n\t"\
		"mull "#x"(%2)\n\t"\
		"movl %%eax, 32(%3)\n\t"\
		"movl %%edx, %%ebx\n\t"\
		__MUL_ASM_UNIT_1__(24,x,28)\
		__MUL_ASM_UNIT_1__(20,x,24)\
		__MUL_ASM_UNIT_1__(16,x,20)\
		__MUL_ASM_UNIT_1__(12,x,16)\
		__MUL_ASM_UNIT_1__(8,x,12)\
		__MUL_ASM_UNIT_1__(4,x,8)\
		"movl (%1), %%eax\n\t"\
		"mull "#x"(%2)\n\t"\
		"addl %%ebx, %%eax\n\t"\
		"movl %%eax, 4(%3)\n\t"\
		"adcl $0, %%edx\n\t"\
		"movl %%edx, (%3)\n\t"

#define __MUL_ASM_UNIT_3__(a,b,c,d,e,f,g,h,i) \
		"movl 32(%3), %%eax\n\t"\
		"addl %%eax, "#a"(%0)\n\t"\
		"movl 28(%3), %%eax\n\t"\
		"adcl %%eax, "#b"(%0)\n\t"\
		"movl 24(%3), %%eax\n\t"\
		"adcl %%eax, "#c"(%0)\n\t"\
		"movl 20(%3), %%eax\n\t"\
		"adcl %%eax, "#d"(%0)\n\t"\
		"movl 16(%3), %%eax\n\t"\
		"adcl %%eax, "#e"(%0)\n\t"\
		"movl 12(%3), %%eax\n\t"\
		"adcl %%eax, "#f"(%0)\n\t"\
		"movl 8(%3), %%eax\n\t"\
		"adcl %%eax, "#g"(%0)\n\t"\
		"movl 4(%3), %%eax\n\t"\
		"adcl %%eax, "#h"(%0)\n\t"\
		"movl (%3), %%eax\n\t"\
		"adcl %%eax, "#i"(%0)\n\t"

void bn_256_mul(BN_512 r, const BN_256 a, const BN_256 b){
	BN_288 t={0};
	r[0]=0;r[1]=0;r[2]=0;r[3]=0;
	r[4]=0;r[5]=0;r[6]=0;r[7]=0;
	r[8]=0;r[9]=0;r[10]=0;r[11]=0;
	r[12]=0;r[13]=0;r[14]=0;r[15]=0;
	asm volatile (
		"movl 28(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"movl %%eax, 60(%0)\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 24(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 56(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 20(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 52(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 16(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 48(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 12(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 44(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 8(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 40(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 4(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 36(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl (%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 32(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, 28(%0)\n\t"
		__MUL_ASM_UNIT_2__(24)
		__MUL_ASM_UNIT_3__(56,52,48,44,40,36,32,28,24)
		__MUL_ASM_UNIT_2__(20)
		__MUL_ASM_UNIT_3__(52,48,44,40,36,32,28,24,20)
		__MUL_ASM_UNIT_2__(16)
		__MUL_ASM_UNIT_3__(48,44,40,36,32,28,24,20,16)
		__MUL_ASM_UNIT_2__(12)
		__MUL_ASM_UNIT_3__(44,40,36,32,28,24,20,16,12)
		__MUL_ASM_UNIT_2__(8)
		__MUL_ASM_UNIT_3__(40,36,32,28,24,20,16,12,8)
		__MUL_ASM_UNIT_2__(4)
		__MUL_ASM_UNIT_3__(36,32,28,24,20,16,12,8,4)
		__MUL_ASM_UNIT_2__()
		__MUL_ASM_UNIT_3__(32,28,24,20,16,12,8,4,)
		::"r"(r),"r"(a),"r"(b),"r"(t)
		:"eax","ebx","edx"
	);
}

void bn_256_imul(BN_256 r, const BN_256 a, const BN_256 b){
	uint32_t t[7];
	r[0]=0;r[1]=0;r[2]=0;r[3]=0;
	r[4]=0;r[5]=0;r[6]=0;r[7]=0;
	asm volatile(
		"movl 28(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"movl %%eax, 28(%0)\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 24(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 24(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 20(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 20(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 16(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 16(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 12(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 12(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 8(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 8(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl 4(%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, 4(%0)\n\t"
		"adcl $0, %%edx\n\t"
		"movl %%edx, %%ebx\n\t"

		"movl (%1), %%eax\n\t"
		"mull 28(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%0)\n\t"
//=====================================
		"movl 28(%1), %%eax\n\t"
		"mull 24(%2)\n\t"
		"movl %%eax, 24(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		__MUL_ASM_UNIT_1__(24,24,20)
		__MUL_ASM_UNIT_1__(20,24,16)
		__MUL_ASM_UNIT_1__(16,24,12)
		__MUL_ASM_UNIT_1__(12,24,8)
		__MUL_ASM_UNIT_1__(8,24,4)
		"movl 4(%1), %%eax\n\t"
		"mull 24(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 24(%3), %%eax\n\t"
		"addl %%eax, 24(%0)\n\t"
		"movl 20(%3), %%eax\n\t"
		"adcl %%eax, 20(%0)\n\t"
		"movl 16(%3), %%eax\n\t"
		"adcl %%eax, 16(%0)\n\t"
		"movl 12(%3), %%eax\n\t"
		"adcl %%eax, 12(%0)\n\t"
		"movl 8(%3), %%eax\n\t"
		"adcl %%eax, 8(%0)\n\t"
		"movl 4(%3), %%eax\n\t"
		"adcl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull 20(%2)\n\t"
		"movl %%eax, 20(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		__MUL_ASM_UNIT_1__(24,20,16)
		__MUL_ASM_UNIT_1__(20,20,12)
		__MUL_ASM_UNIT_1__(16,20,8)
		__MUL_ASM_UNIT_1__(12,20,4)
		"movl 8(%1), %%eax\n\t"
		"mull 20(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 20(%3), %%eax\n\t"
		"addl %%eax, 20(%0)\n\t"
		"movl 16(%3), %%eax\n\t"
		"adcl %%eax, 16(%0)\n\t"
		"movl 12(%3), %%eax\n\t"
		"adcl %%eax, 12(%0)\n\t"
		"movl 8(%3), %%eax\n\t"
		"adcl %%eax, 8(%0)\n\t"
		"movl 4(%3), %%eax\n\t"
		"adcl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull 16(%2)\n\t"
		"movl %%eax, 16(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		__MUL_ASM_UNIT_1__(24,16,12)
		__MUL_ASM_UNIT_1__(20,16,8)
		__MUL_ASM_UNIT_1__(16,16,4)
		"movl 12(%1), %%eax\n\t"
		"mull 16(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 16(%3), %%eax\n\t"
		"addl %%eax, 16(%0)\n\t"
		"movl 12(%3), %%eax\n\t"
		"adcl %%eax, 12(%0)\n\t"
		"movl 8(%3), %%eax\n\t"
		"adcl %%eax, 8(%0)\n\t"
		"movl 4(%3), %%eax\n\t"
		"adcl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull 12(%2)\n\t"
		"movl %%eax, 12(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		__MUL_ASM_UNIT_1__(24,12,8)
		__MUL_ASM_UNIT_1__(20,12,4)
		"movl 16(%1), %%eax\n\t"
		"mull 12(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 12(%3), %%eax\n\t"
		"addl %%eax, 12(%0)\n\t"
		"movl 8(%3), %%eax\n\t"
		"adcl %%eax, 8(%0)\n\t"
		"movl 4(%3), %%eax\n\t"
		"adcl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull 8(%2)\n\t"
		"movl %%eax, 8(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		__MUL_ASM_UNIT_1__(24,8,4)
		"movl 20(%1), %%eax\n\t"
		"mull 8(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 8(%3), %%eax\n\t"
		"addl %%eax, 8(%0)\n\t"
		"movl 4(%3), %%eax\n\t"
		"adcl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull 4(%2)\n\t"
		"movl %%eax, 4(%3)\n\t"
		"movl %%edx, %%ebx\n\t"
		"movl 24(%1), %%eax\n\t"
		"mull 4(%2)\n\t"
		"addl %%ebx, %%eax\n\t"
		"movl %%eax, (%3)\n\t"

		"movl 4(%3), %%eax\n\t"
		"addl %%eax, 4(%0)\n\t"
		"movl 0(%3), %%eax\n\t"
		"adcl %%eax, (%0)\n\t"
//===============================
		"movl 28(%1), %%eax\n\t"
		"mull (%2)\n\t"
		"addl %%eax, (%0)\n\t"
		::"r"(r),"r"(a),"r"(b),"r"(t)
		:"eax","ebx","edx"
	);
}

void bn_256_cpy(BN_256 r, const BN_256 a){
	r[0]=a[0];r[1]=a[1];
	r[2]=a[2];r[3]=a[3];
	r[4]=a[4];r[5]=a[5];
	r[6]=a[6];r[7]=a[7];
}

#undef __MUL_ASM_UNIT_1__
#undef __MUL_ASM_UNIT_2__
#undef __MUL_ASM_UNIT_3__

/* r=a-b，取512位数ab的低288位，将其相减 */
void __bn_288_sub(BN_512 r, const BN_512 a, const BN_512 b){
	asm volatile (
		"movl 60(%1), %%eax\n\t"
		"subl 60(%2), %%eax\n\t"
		"movl %%eax, 60(%0)\n\t"
		"movl 56(%1), %%eax\n\t"
		"sbbl 56(%2), %%eax\n\t"
		"movl %%eax, 56(%0)\n\t"
		"movl 52(%1), %%eax\n\t"
		"sbbl 52(%2), %%eax\n\t"
		"movl %%eax, 52(%0)\n\t"
		"movl 48(%1), %%eax\n\t"
		"sbbl 48(%2), %%eax\n\t"
		"movl %%eax, 48(%0)\n\t"
		"movl 44(%1), %%eax\n\t"
		"sbbl 44(%2), %%eax\n\t"
		"movl %%eax, 44(%0)\n\t"
		"movl 40(%1), %%eax\n\t"
		"sbbl 40(%2), %%eax\n\t"
		"movl %%eax, 40(%0)\n\t"
		"movl 36(%1), %%eax\n\t"
		"sbbl 36(%2), %%eax\n\t"
		"movl %%eax, 36(%0)\n\t"
		"movl 32(%1), %%eax\n\t"
		"sbbl 32(%2), %%eax\n\t"
		"movl %%eax, 32(%0)\n\t"
		"movl 28(%1), %%eax\n\t"
		"sbbl 28(%2), %%eax\n\t"
		"movl %%eax, 28(%0)\n\t"
		::"r"(r),"r"(a),"r"(b)
		:"eax"
	);
}

/* r=r-b，取512的低288位，将其减去一个256位 */
void __bn_288_sbb_256(BN_512 r, const BN_256 b){
	asm volatile(
		"movl 28(%1), %%eax\n\t"
		"subl %%eax, 60(%0)\n\t"
		"movl 24(%1), %%eax\n\t"
		"sbbl %%eax, 56(%0)\n\t"
		"movl 20(%1), %%eax\n\t"
		"sbbl %%eax, 52(%0)\n\t"
		"movl 16(%1), %%eax\n\t"
		"sbbl %%eax, 48(%0)\n\t"
		"movl 12(%1), %%eax\n\t"
		"sbbl %%eax, 44(%0)\n\t"
		"movl 8(%1), %%eax\n\t"
		"sbbl %%eax, 40(%0)\n\t"
		"movl 4(%1), %%eax\n\t"
		"sbbl %%eax, 36(%0)\n\t"
		"movl (%1), %%eax\n\t"
		"sbbl %%eax, 32(%0)\n\t"
		"sbbl $0, 28(%0)\n\t"
		::"r"(r),"r"(b)
		:"eax"
	);
}

/* 将一个512位的低288位与一个256位比较，256位数视为前方补零的288位数 */
int __bn_288_cmp_256(const BN_512 a, const BN_256 b){
	if(a[7]>0){
		return 1;
	}
	return bn_256_cmp(bn_512_l256(a),b);
}

/* 将两个512位的高256位相加，还要加上一位进位，因为已知a+b的结果低256位为0 */
int __bn_512_h256_adc(BN_256 r, const BN_512 a, const BN_512 b){
	int f=0;
	asm volatile(
		"stc\n\t"
		__ADC_ASM_UNIT_1__(28)
		__ADC_ASM_UNIT_1__(24)
		__ADC_ASM_UNIT_1__(20)
		__ADC_ASM_UNIT_1__(16)
		__ADC_ASM_UNIT_1__(12)
		__ADC_ASM_UNIT_1__(8)
		__ADC_ASM_UNIT_1__(4)
		__ADC_ASM_UNIT_1__()
		"movl $0, %0\n\t"
		"adcl $0, %0\n\t"
		:"=r"(f)
		:"r"(r),"r"(a),"r"(b)
		:"eax"
	);
	return f;
}
#undef __ADC_ASM_UNIT_1__
