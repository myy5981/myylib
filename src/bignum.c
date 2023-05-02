#include <myy/bignum.h>

void bn_add_256(BN_256 r, BN_256 a, BN_256 b){
    __asm__ volatile (
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

int bin_2_bn_256(BN_256 bn, const char* bin){
    int len=0;
    char c;
    while((c=bin[len])!='\0'){
        if(c!='0'&&c!='1'){
            return -1;
        }
        len++;
    }
    uint32_t n=0;int j=0,k=7;
    for(int i=len-1;i>=0;--i){
        if(bin[i]=='1'){
            n=n+(1<<j);
        }
        j++;
        if(j==32){
            j=0;
            bn[k--]=n;
            if(k<0){
                return 0;
            }
            n=0;
        }
    }
    bn[k]=n;
    return 0;
}

void bn_2_bin_256(BN_256 bn, char* bin){
    int k=0;
    for (int i = 0; i < 8; i++) {
		uint32_t n = bn[i];
		for (int j = 31; j >= 0; j--) {
            bin[k++]=((n>>j)&1)==0?'0':'1';
		}
	}
}

void bn_zero_256(BN_256 bn){
    bn[0] = 0; bn[1] = 0;
    bn[2] = 0; bn[3] = 0;
    bn[4] = 0; bn[5] = 0;
    bn[6] = 0; bn[7] = 0;
}
