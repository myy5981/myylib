#include <myy/bignum.h>
#include <stdio.h>

int main(void){
    const char* _a="101010010111111111101010100010010100101";
    const char* _b="101000110100100100010001001001111111111";
    //00000000000001010011001100100011111011101100010100100
    BN_256 a=BN_ZERO_256;
    BN_256 b=BN_ZERO_256;
    BN_256 c=BN_ZERO_256;

    bin_2_bn_256(a,_a);
    bin_2_bn_256(b,_b);

    bn_add_256(c,a,b);

    char res[257]={0};
    bn_2_bin_256(c,res);

    printf("%s\n",res);
    return 0;
}