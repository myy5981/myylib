#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>

#include <stdio.h>
#include <string.h>

#include <openssl/bn.h>

//#include <gmssl/sm2.h>

#include <sys/time.h>
#define TIMES 1

char* str="BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";
char* strp="FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";

int main(void){
	struct timeval begin;
	struct timeval end;

	BN_256 a=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0);

	uint8_t abin[32]={0};

	BN_256_GFp_Mont A={0};
	BN_256_GFp_Mont r={0};

	bn_256_GFp_to_mont(A,a);

	//bn_256_GFp_mul_mont(A,A,A);

	gettimeofday(&begin,NULL);

	int i = bn_256_GFp_sqrt_mont(r,A);

	gettimeofday(&end,NULL);
	printf("myylib cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	printf("resi=%d\n",i);

	bn_256_mont_to_GFp(a,r);
	bn_256_to_bin(a,abin);
	hex_enc2stream(stdout,abin,32);
	printf("\n");

	BIGNUM* o=BN_new();
	BIGNUM* p=BN_new();
	BIGNUM* or=BN_new();
	BN_CTX* ctx=BN_CTX_new();

	BN_hex2bn(&o,str);
	BN_hex2bn(&p,strp);

	gettimeofday(&begin,NULL);

	BN_mod_sqrt(or,o,p,ctx);

	gettimeofday(&end,NULL);
	printf("openssl cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	printf("\n%s\n",BN_bn2hex(or));
}