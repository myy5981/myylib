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
	int i;
	struct timeval begin;
	struct timeval end;

	SM2_PRI_KEY key;
	SM2_PRI_KEY_EXT kext;
	SM3_CTX _ctx,*ctx=&_ctx;
	uint8_t hash[32]={0};

	SM2_SIGNATURE sign;

	uint8_t s[64];

	BN_256 k=BN_256_INIT(3945208F,7B2144B1,3F36E38A,C6D39F95,88939369,2860B51A,42FB81EF,4DF7C5B8);
	bn_256_cpy(key.d,k);
	SM2_JPOINT pub;
	sm2_point_mul_mont(&pub,&SM2_G_MONT,key.d);
	sm2_jpoint_to_point_mont(&(key.Pub),&pub);
	sm2_point_from_mont(&(key.Pub),&(key.Pub));

	// sm2_key_generate(&key);
	sm2_key_extend(&kext,&key);

	i=sm2_sig_init(ctx,&(key.Pub),(uint8_t*)"1234567812345678",16);
	if(i!=0){
		printf("err1\n");
		return 0;
	}
	i=sm2_sig_update(ctx,(uint8_t*)"message digest",14);	
	if(i!=0){
		printf("err2\n");
		return 0;
	}
	sm2_sig_final(ctx,hash);

	gettimeofday(&begin,NULL);

	sm2_sig_generate(&sign,&kext,hash);

	gettimeofday(&end,NULL);
	printf("myylib cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	sm2_sig_to_bin(&sign,s);
	printf("\n");
	hex_enc2stream(stdout,s,64);
	printf("\n");
	sm2_sig_from_bin(&sign,s);

	gettimeofday(&begin,NULL);

	i=sm2_sig_verify(&sign,&(key.Pub),hash);

	gettimeofday(&end,NULL);
	printf("myylib cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	printf("resi=%d\n",i);

}