#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>
#include "task.h"
#include <stdio.h>
#include <string.h>

char* str="BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";
char* strp="FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";

SM2_PRI_KEY key;
SM2_PRI_KEY_EXT kext;
SM3_CTX _ctx,*ctx=&_ctx;
uint8_t hash[32]={0};
SM2_SIGNATURE sign;
uint8_t s[64];
SM2_JPOINT pub;
BN_256 k=BN_256_INIT(3945208F,7B2144B1,3F36E38A,C6D39F95,88939369,2860B51A,42FB81EF,4DF7C5B8);

int res;

int before(){
	int i;
	bn_256_cpy(key.d,k);
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

	return 1;
}

void fun(){
	sm2_sig_generate(&sign,&kext,hash);
}

void after(){
	sm2_sig_to_bin(&sign,s);
	printf("\n");
	hex_enc2stream(stdout,s,64);
	printf("\n");
	sm2_sig_from_bin(&sign,s);
}

void fun2(){
	res = sm2_sig_verify(&sign,&(key.Pub),hash);
}

void after2(){
	printf("resi=%d\n",res);
}

const Task tasks[]={
	{.after=after,.before=before,.task=fun},
	{.after=after2,.before=NULL,.task=fun2}
};
int tasks_len=2;
