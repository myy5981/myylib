#include <myy/bignum.h>
#include <myy/sm2_bn.h>
#include <myy/hex.h>

#include <stdio.h>

#include <openssl/bn.h>

#define BIGNUM_INIT(a,b,c,d,e,f,g,h,bn) BN_hex2bn(&bn,#a#b#c#d#e#f#g#h)

int main(void){
    BN_CTX* ctx=BN_CTX_new();
    BIGNUM* a_=BN_new();
    BIGNUM* b_=BN_new();
    BIGNUM* m=BN_new();

    BN_hex2bn(&m,"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF");
    
    BIGNUM* r3=BN_new();

    BN_256 a=   BN_256_INIT(FFFFFFFE,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000004);
                BIGNUM_INIT(FFFFFFFE,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000004,a_);
    
    BN_256 b=   BN_256_INIT(FFFFFFFE,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,0000000E);
                BIGNUM_INIT(FFFFFFFE,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,0000000E,b_);

    BN_256 r={0};
    bn_256_GFp_mul(r,a,b);
    for (int i = 0; i < 8; i++)
    {
        printf("%8X",r[i]);
    }
    printf("\n");
    

    BN_mod_mul(r3,a_,b_,m,ctx);
    printf("000000%s\n",BN_bn2hex(r3));

    return 0;
}