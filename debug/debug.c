#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>

#include <stdio.h>

//#include <openssl/bn.h>

//#include <gmssl/sm2.h>

#include <sys/time.h>
#define TIMES 1

static const JACOBIAN_POINT SM2_G={
	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
	.z=BN_256_ONE
};

//char* as="32c4ae2c1f1981195f9904466a39c9948fe30bbff2660be1715a4589334c74c7";
//char* bs="bc3736a2f4f6779c59bdcee36b692153d0a9877cc62a474002df32e52139f0a0";
int main(void){
	struct timeval begin;
	struct timeval end;

	uint8_t res[32]={0};
	char hex[65]={0};

	JACOBIAN_POINT R;
	SM2_POINT r;

	gettimeofday(&begin,NULL);
	for (int i = 0; i < TIMES; i++)
	{
		sm2_point_dbl(&R,&SM2_G);
	}
	
	gettimeofday(&end,NULL);
	printf("myylibc cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	sm2_jacobian_to_point(&r,&R);
	bn_256_to_bin(r.x,res);
	hex_encode(res,32,hex,HEX_ENCODE_LOWER);
	printf("x=%s\n",hex);
	sm2_jacobian_to_point(&r,&R);
	bn_256_to_bin(r.y,res);
	hex_encode(res,32,hex,HEX_ENCODE_LOWER);
	printf("y=%s\n",hex);

	// SM2_BN a2={0};
	// SM2_BN b2={0};
	// SM2_BN r2={0};
	
	// sm2_bn_from_hex(a2,as);
	// sm2_bn_from_hex(b2,bs);


	// gettimeofday(&begin,NULL);
	// for (int i = 0; i < TIMES; i++)
	// {
	// sm2_fp_inv(r2,a2);
	// }
	
	// gettimeofday(&end,NULL);
	// printf("  gmssl cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	// sm2_bn_to_hex(r2,hex);
	// printf("  gmssl:%s\n",hex);

	return 0;

}