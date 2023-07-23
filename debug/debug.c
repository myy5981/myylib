#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>

#include <stdio.h>

//#include <openssl/bn.h>

//#include <gmssl/sm2.h>

#include <sys/time.h>
#define TIMES 1

// static const SM2_JPOINT SM2_G={
// 	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
// 	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
// 	.z=BN_256_ONE
// };
static const SM2_POINT __SM2_G={
	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
};
static const BN_256 __SM2_B=BN_256_INIT(28E9FA9E,9D9F5E34,4D5A9E4B,CF6509A7,F39789F5,15AB8F92,DDBCBD41,4D940E93);
int main(void){
	struct timeval begin;
	struct timeval end;

	uint8_t res[32]={0};
	char hex[65]={0};

	SM2_JPOINT R;
	SM2_POINT r;

	gettimeofday(&begin,NULL);
	for (int i = 0; i < TIMES; i++)
	{
		sm2_point_mul(&R,&__SM2_G,__SM2_B);
	}
	
	gettimeofday(&end,NULL);
	printf("myylibc cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	sm2_jpoint_to_point(&r,&R);
	bn_256_to_bin(r.x,res);
	hex_encode(res,32,hex,HEX_ENCODE_LOWER);
	printf("x=%s\n",hex);
	bn_256_to_bin(r.y,res);
	hex_encode(res,32,hex,HEX_ENCODE_LOWER);
	printf("y=%s\n",hex);
}