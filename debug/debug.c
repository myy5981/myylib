#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>

#include <stdio.h>
#include <string.h>

//#include <openssl/bn.h>

//#include <gmssl/sm2.h>

#include <sys/time.h>
#define TIMES 1

char* str="01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345";

int main(void){
	struct timeval begin;
	struct timeval end;

	SM2_PRI_KEY key;
	BN_256 k=BN_256_INIT(3945208F,7B2144B1,3F36E38A,C6D39F95,88939369,2860B51A,42FB81EF,4DF7C5B8);
	bn_256_cpy(key.d,k);
	SM2_JPOINT pub;
	sm2_point_mul_mont(&pub,&SM2_G_MONT,key.d);
	sm2_jpoint_to_point_mont(&(key.Pub),&pub);
	sm2_point_from_mont(&(key.Pub),&(key.Pub));

	uint8_t buf[1024]={0};

	gettimeofday(&begin,NULL);

	int r = sm2_encrypt(&(key.Pub),(uint8_t*)str,strlen(str),buf);

	char m[1024]={0};
	int ret = sm2_decrypt(&key,buf,r,(uint8_t*)m);
	
	gettimeofday(&end,NULL);
	printf("myylibc cost:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);

	printf("enc %d bytes\n",r);

	hex_enc2stream(stdout,buf,1);
	printf("\n");
	hex_enc2stream(stdout,buf+1,r-1);
	printf("\n");

	printf("de %d bytes:%s\n",ret,m);
}