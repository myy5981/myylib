#include <myy/bignum.h>
#include <myy/endian.h>

void __bn_256_add(BN_256 r, const BN_256 a, const BN_256 b){
    asm volatile (
        "movl 28(%1), %%eax\n\t"
        "addl 28(%2), %%eax\n\t"
        "movl %%eax, 28(%0)\n\t"
        "movl 24(%1), %%eax\n\t"
        "adcl 24(%2), %%eax\n\t"
        "movl %%eax, 24(%0)\n\t"
        "movl 20(%1), %%eax\n\t"
        "adcl 20(%2), %%eax\n\t"
        "movl %%eax, 20(%0)\n\t"
        "movl 16(%1), %%eax\n\t"
        "adcl 16(%2), %%eax\n\t"
        "movl %%eax, 16(%0)\n\t"
        "movl 12(%1), %%eax\n\t"
        "adcl 12(%2), %%eax\n\t"
        "movl %%eax, 12(%0)\n\t"
        "movl 8(%1), %%eax\n\t"
        "adcl 8(%2), %%eax\n\t"
        "movl %%eax, 8(%0)\n\t"
        "movl 4(%1), %%eax\n\t"
        "adcl 4(%2), %%eax\n\t"
        "movl %%eax, 4(%0)\n\t"
        "movl (%1), %%eax\n\t"
        "adcl (%2), %%eax\n\t"
        "movl %%eax, (%0)\n\t"
        ::"r"(r),"r"(a),"r"(b)
        :"eax"
    );
}

int bn_256_add(BN_256 r, const BN_256 a, const BN_256 b){
    int f=0;
    asm volatile (
        "movl 28(%2), %%eax\n\t"
        "addl 28(%3), %%eax\n\t"
        "movl %%eax, 28(%1)\n\t"
        "movl 24(%2), %%eax\n\t"
        "adcl 24(%3), %%eax\n\t"
        "movl %%eax, 24(%1)\n\t"
        "movl 20(%2), %%eax\n\t"
        "adcl 20(%3), %%eax\n\t"
        "movl %%eax, 20(%1)\n\t"
        "movl 16(%2), %%eax\n\t"
        "adcl 16(%3), %%eax\n\t"
        "movl %%eax, 16(%1)\n\t"
        "movl 12(%2), %%eax\n\t"
        "adcl 12(%3), %%eax\n\t"
        "movl %%eax, 12(%1)\n\t"
        "movl 8(%2), %%eax\n\t"
        "adcl 8(%3), %%eax\n\t"
        "movl %%eax, 8(%1)\n\t"
        "movl 4(%2), %%eax\n\t"
        "adcl 4(%3), %%eax\n\t"
        "movl %%eax, 4(%1)\n\t"
        "movl (%2), %%eax\n\t"
        "adcl (%3), %%eax\n\t"
        "movl %%eax, (%1)\n\t"
        "movl $0, %0\n\t"
        "adcl $0, %0\n\t"
        :"=r"(f)
        :"r"(r),"r"(a),"r"(b)
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

void bn_256_to_bytes (BN_256 bn, uint8_t* dest){
    ((uint32_t*)dest)[0]=h2be_32(bn[0]);
    ((uint32_t*)dest)[1]=h2be_32(bn[1]);
    ((uint32_t*)dest)[2]=h2be_32(bn[2]);
    ((uint32_t*)dest)[3]=h2be_32(bn[3]);
    ((uint32_t*)dest)[4]=h2be_32(bn[4]);
    ((uint32_t*)dest)[5]=h2be_32(bn[5]);
    ((uint32_t*)dest)[6]=h2be_32(bn[6]);
    ((uint32_t*)dest)[7]=h2be_32(bn[7]);
}

#define __MUL_ASM_UNIT_1__(a,x,t) \
        "movl "#a"(%1), %%eax\n\t"\
        "mull "#x"(%2)\n\t"\
        "addl %%ebx, %%eax\n\t"\
        "movl %%eax, "#t"(%3)\n\t"\
        "adcl $0, %%edx\n\t"\
        "movl %%edx, %%ebx\n\t"\

#define __MUL_ASM_UNIT_2__(x)         \
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
    uint32_t t[9]={0};
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

void bn_256_dmul(BN_256 r, const BN_256 a, const BN_256 b){
    BN_512 _r;
    bn_256_mul(_r,a,b);
    bn_512_high(r,_r);
}

/*

--------|--------
       J|ABCDEFGH
      JA|BCDEFGH
     JAB|CDEFGH
    JABC|DEFGH
   JABCD|EFGH
  JABCDE|FGH
 JABCDEF|GH
JABCDEFG|H
*/


#undef __MUL_ASM_UNIT_1__
#undef __MUL_ASM_UNIT_2__
#undef __MUL_ASM_UNIT_3__

void bn_288_sub(BN_512 r, const BN_512 a, const BN_512 b){
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

int bn_288_cmp(const BN_512 a, const BN_512 b){
    for (int i=7;i<16;i++){
        if(a[i]>b[i]) return 1;
        if(a[i]<b[i]) return -1;
    }
    return 0;
}

void bn_256_from_bytes (BN_256 bn, uint8_t* src){
    bn[0]=be2h_32(((uint32_t*)src)[0]);
    bn[1]=be2h_32(((uint32_t*)src)[1]);
    bn[2]=be2h_32(((uint32_t*)src)[2]);
    bn[3]=be2h_32(((uint32_t*)src)[3]);
    bn[4]=be2h_32(((uint32_t*)src)[4]);
    bn[5]=be2h_32(((uint32_t*)src)[5]);
    bn[6]=be2h_32(((uint32_t*)src)[6]);
    bn[7]=be2h_32(((uint32_t*)src)[7]);
}
