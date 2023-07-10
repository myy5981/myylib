#include <myy/bignum.h>
#include <myy/sm2_bn.h>
#include <myy/hex.h>

#include <stdio.h>
int main(void){

BN_256_GFp r=BN_256_ZERO;
BN_256_GFp a=BN_256_INIT(fffc73ab,d92efb9c,fad6ebfc,6d7eabf6,e7a95792,33000000,00000000,00000000);
BN_256_GFp b=BN_256_INIT(f8796103,7490aefd,bcfecbab,dfeefa87,54957765,70000000,00000000,00000000);

bn_256_GFp_add(r,a,b);

uint8_t bn[32]={0};
char hex[65]={0};

    bn_256_to_bytes(r,bn);

    hex_encode(bn,32,hex,HEX_ENCODE_UPPER);

    printf("%s\n",hex);

bn_256_add(r,a,b);

    bn_256_to_bytes(r,bn);

    hex_encode(bn,32,hex,HEX_ENCODE_UPPER);

    printf("%s\n",hex);

    return 0;
}