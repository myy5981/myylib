#include <myy/bignum.h>
#include <myy/sm2_bn.h>
#include <myy/hex.h>

#include <stdio.h>

#include <openssl/bn.h>

#include <gmssl/sm2.h>

#include <sys/time.h>

char* as="32c4ae2c1f1981195f9904466a39c9948fe30bbff2660be1715a4589334c74c7";
char* bs="bc3736a2f4f6779c59bdcee36b692153d0a9877cc62a474002df32e52139f0a0";
#define TIMES 1000000
int main(void){

    struct timeval begin;
    struct timeval end;

    uint8_t res[32]={0};
    char hex[65]={0};

    BN_CTX* ctx=BN_CTX_new();
    BIGNUM* a1=BN_new();
    BIGNUM* b1=BN_new();
    BIGNUM* r1=BN_new();
    BIGNUM* P=BN_new();
    BN_hex2bn(&a1,as);
    BN_hex2bn(&b1,bs);
    BN_hex2bn(&P,"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF");

    gettimeofday(&begin,NULL);    
    for (int i = 0; i < TIMES; i++)
    {
    BN_mod_mul(r1,a1,b1,P,ctx);
    }
    gettimeofday(&end,NULL);
    printf("openssl cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

    printf("openssl:%s\n",BN_bn2hex(r1));
    BN_256 a,b,r;
    uint8_t abin[32];
    uint8_t bbin[32];
    hex_decode(as,64,abin,HEX_DECODE_STRICT);
    bn_256_from_bytes(a,abin);
    hex_decode(bs,64,bbin,HEX_DECODE_STRICT);
    bn_256_from_bytes(b,bbin);

    gettimeofday(&begin,NULL);
    for (int i = 0; i < TIMES; i++)
    {
    bn_256_GFp_mul(r,a,b);
    }
    
    gettimeofday(&end,NULL);
    printf("myylibc cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

    bn_256_to_bytes(r,res);
    hex_encode(res,32,hex,HEX_ENCODE_UPPER);
    printf("myylibc:%s\n",hex);

    SM2_BN a2={0};
    SM2_BN b2={0};
    SM2_BN r2={0};
    
    sm2_bn_from_hex(a2,as);
    sm2_bn_from_hex(b2,bs);


    gettimeofday(&begin,NULL);
    for (int i = 0; i < TIMES; i++)
    {
    sm2_fp_mul(r2,a2,b2);
    }
    
    gettimeofday(&end,NULL);
    printf("  gmssl cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

    sm2_bn_to_hex(r2,hex);
    printf("  gmssl:%s\n",hex);

    return 0;
}