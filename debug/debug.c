#include <myy/bignum.h>
#include <stdio.h>

int main(void){


    unsigned int i=1;
    unsigned int j=0xFFFFFFFC;
    int k=i-j;
    printf("%d,%u,%x\n",i,i,i);
    printf("%d,%u,%x\n",j,j,j);
    printf("%d,%u,%x\n",k,k,k);

    // const char* _a="101010010111111111101010100010010100101";
    // const char* _b="101000110100100100010001001001111111111";
    // //00000000000000000001100011011011011001011000010100110
    // //11111111111111111110011100100100100110100111101011010
    // //00000000000001010011001100100011111011101100010100100
    // BN_256 a=BN_ZERO_256;
    // BN_256 b=BN_ZERO_256;
    // BN_256 c=BN_ZERO_256;
    // bin_2_bn_256(a,_a);
    // bin_2_bn_256(b,_b);

    // bn_sub_256(c,b,a);

    // char res[257]={0};
    // bn_2_bin_256(c,res);

    // printf("\n%s\n",res);

    return 0;
}