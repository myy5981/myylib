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

int  bn_256_add(BN_256 r, const BN_256 a, const BN_256 b){
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

void bn_256_zero(BN_256 bn){
    bn[0] = 0; bn[1] = 0;
    bn[2] = 0; bn[3] = 0;
    bn[4] = 0; bn[5] = 0;
    bn[6] = 0; bn[7] = 0;
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
