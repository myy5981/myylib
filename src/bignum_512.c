#include <myy/bignum.h>

void bn_256_mul(BN_512 r, BN_256 a, BN_256 b){
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
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 24(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 56(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 52(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 48(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 44(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 40(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 36(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 20(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 52(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 48(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 44(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 40(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 36(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 20(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 16(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 48(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 44(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 40(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 36(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 20(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 16(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 12(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 44(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 40(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 36(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 20(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 16(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 12(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 8(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 40(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 36(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 20(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 16(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 12(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 8(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull 4(%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 36(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 32(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 20(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
        "movl 16(%3), %%eax\n\t"
        "adcl %%eax, 20(%0)\n\t"
        "movl 12(%3), %%eax\n\t"
        "adcl %%eax, 16(%0)\n\t"
        "movl 8(%3), %%eax\n\t"
        "adcl %%eax, 12(%0)\n\t"
        "movl 4(%3), %%eax\n\t"
        "adcl %%eax, 8(%0)\n\t"
        "movl (%3), %%eax\n\t"
        "adcl %%eax, 4(%0)\n\t"
/*======================================*/
        "movl 28(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "movl %%eax, 32(%3)\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 24(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 28(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 20(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 24(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 16(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 20(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 12(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 16(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 8(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 12(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl 4(%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 8(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, %%ebx\n\t"

        "movl (%1), %%eax\n\t"
        "mull (%2)\n\t"
        "addl %%ebx, %%eax\n\t"
        "movl %%eax, 4(%3)\n\t"
        "adcl $0, %%edx\n\t"
        "movl %%edx, (%3)\n\t"

        "movl 32(%3), %%eax\n\t"
        "addl %%eax, 32(%0)\n\t"
        "movl 28(%3), %%eax\n\t"
        "adcl %%eax, 28(%0)\n\t"
        "movl 24(%3), %%eax\n\t"
        "adcl %%eax, 24(%0)\n\t"
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
        "movl (%3), %%eax\n\t"
        "adcl %%eax, (%0)\n\t"
/*======================================*/
        ::"r"(r),"r"(a),"r"(b),"r"(t)
        :"eax","ebx","edx"
    );
}
