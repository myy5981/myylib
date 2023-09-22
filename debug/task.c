#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>
#include <myy/sm4.h>
#include <myy/random.h>
#include "task.h"
#include <stdio.h>
#include <string.h>

#define MLEN 19
SM2_PRI_KEY m_key;
SM2_PRI_KEY_EXT m_key_ext;
SM2_SIGNATURE m_sign;
uint8_t m[MLEN]={0};
uint8_t d[MLEN]={0};
uint8_t c[MLEN+65+32]={0};
uint8_t hash[32]={0};
int r;

int myy_before(){
	rand_bytes(m,MLEN);
	rand_bytes(hash,32);
	return 1;
}
void myy_key_gen(){
	sm2_key_generate(&m_key,NULL);
}

void myy_enc(){
	r = sm2_encrypt(&(m_key.Pub),m,MLEN,c);
}
void myy_dec(){
	r = sm2_decrypt(&m_key,c,r,d);
}
void myy_dec_after(){
	printf("%d %d\n",r,memcmp(m,d,MLEN));
}
int myy_sign_before(){
	sm2_key_extend(&m_key_ext,&m_key);
	return 1;
}
void myy_sign(){
	sm2_sig_generate(&m_sign,&m_key_ext,hash);
}
void myy_sig_ver(){
	r = sm2_sig_verify(&m_sign,&(m_key.Pub),hash);
}
void myy_sign_after(){
	printf("r=%d\n",r);
}

const Task tasks[]={
	{.before=myy_before,.task=myy_key_gen,.after=NULL},
	{.before=NULL,.task=myy_enc,.after=NULL},
	{.before=NULL,.task=myy_dec,.after=myy_dec_after},
	{.before=myy_sign_before,.task=myy_sign,.after=NULL},
	{.before=NULL,.task=myy_sig_ver,.after=myy_sign_after},
	{.before=myy_before,.task=myy_key_gen,.after=NULL},
	{.before=NULL,.task=myy_enc,.after=NULL},
	{.before=NULL,.task=myy_dec,.after=myy_dec_after},
	{.before=myy_sign_before,.task=myy_sign,.after=NULL},
	{.before=NULL,.task=myy_sig_ver,.after=myy_sign_after}
};
int tasks_len=10;