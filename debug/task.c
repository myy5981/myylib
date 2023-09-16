#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>
#include <myy/sm4.h>
#include <myy/random.h>
#include "task.h"
#include <stdio.h>
#include <string.h>

int res1,res2;

SM2_PRI_KEY _A,*A=&_A;
SM2_PRI_KEY _B,*B=&_B;
SM2_PRI_KEY _rA,*rA=&_rA;
SM2_PRI_KEY _rB,*rB=&_rB;
uint8_t Za[32];
uint8_t Zb[32];

SM3_KDF_CTX ka,kb;

BN_256 da=BN_256_INIT(81EB26E9,41BB5AF1,6DF11649,5F906952,72AE2CD6,3D6C4AE1,678418BE,48230029);
BN_256 db=BN_256_INIT(78512991,7D45A9EA,5437A593,56B82338,EAADDA6C,EB199088,F14AE10D,EFA229B5);
BN_256 ra=BN_256_INIT(D4DE1547,4DB74D06,491C440D,305E0124,00990F3E,390C7E87,153C12DB,2EA60BB3);
BN_256 rb=BN_256_INIT(7E071248,14B30948,9125EAED,10111316,4EBF0F34,58C5BD88,335C1F9D,596243D6);

/*
c558b44bee5301d9f52b44d939bb59584d75b9034dd6a9fc826872109a65739f
3252b35b191d8ae01cd122c025204334c5eacf68a0cb4854c6a7d367ecad4de7
3b85a57179e11e7e513aa622991f2ca74d1807a0bd4d4b38f90987a17ac245b1
79c988d63229d97ef19fe02ca1056e01e6a7411ed24694aa8f834f4a4ab022f7
*/

int before(){
	sm2_key_generate(A,da);
	sm2_key_generate(B,db);
	sm2_key_generate(rA,ra);
	sm2_key_generate(rB,rb);

	sm2_id_generate(Za,&(A->Pub),NULL,7);
	sm2_id_generate(Zb,&(B->Pub),NULL,7);

	return 1;
}

void fun(){
	res1 = sm2_ecmqv_init(&ka,A,rA,&(B->Pub),&(rB->Pub),Za,Zb);
	res2 = sm2_ecmqv_init(&kb,B,rB,&(A->Pub),&(rA->Pub),Za,Zb);
}

void after(){
	printf("%d,%d\n",res1,res2);

	uint8_t buf[16];
	sm3_kdf_final(&ka,buf,16);
	hex_enc2stream(stdout,buf,16);
	sm3_kdf_final(&kb,buf,16);
	hex_enc2stream(stdout,buf,16);
}

void test_sm4_gcm(){
	SM4_GCM_CTX ctx;
	uint8_t key[16]={0};
	uint8_t IV[12]={0};
	uint8_t hash[16];
	uint8_t c[16]={0};
	uint8_t m[16]={0};
	sm4_gcm_init(&ctx,key,IV,NULL,0);
	sm4_gcm_enc_final(&ctx,c,hash,16);

	printf("C_1: ");
	hex_enc2stream(stdout,c,16);
	printf("T_1: ");
	hex_enc2stream(stdout,hash,16);

	sm4_gcm_init(&ctx,key,IV,NULL,0);
	sm4_gcm_enc_update(&ctx,c,m,16);
	sm4_gcm_enc_final(&ctx,c,hash,16);

	printf("C_2: ");
	hex_enc2stream(stdout,c,16);
	printf("T_2: ");
	hex_enc2stream(stdout,hash,16);
}

void test_sm4_gcm_dec(){
	#define MESSAGE_LEN 64
	SM4_GCM_CTX ctx;
	uint8_t key[16]={0};
	uint8_t IV[12]={0};
	uint8_t m[MESSAGE_LEN]={0};
	uint8_t c[MESSAGE_LEN]={0};
	uint8_t d[MESSAGE_LEN]={0};
	uint8_t gmac[16];

	rand_bytes(key,16);
	rand_bytes(IV,12);
	rand_bytes(m,MESSAGE_LEN);

	sm4_gcm_init(&ctx,key,IV,(const uint8_t*)"myy5981@outlook.com",19);
	int r=sm4_gcm_enc_update(&ctx,c,m,MESSAGE_LEN);
	sm4_gcm_enc_final(&ctx,c+r,gmac,16);

	printf("key: ");
	hex_enc2stream(stdout,key,16);
	printf(" IV: ");
	hex_enc2stream(stdout,IV,12);
	printf("  m: ");
	hex_enc2stream(stdout,m,MESSAGE_LEN);
	printf("  c: ");
	hex_enc2stream(stdout,c,MESSAGE_LEN);
	printf("mac: ");
	hex_enc2stream(stdout,gmac,16);

	sm4_gcm_reset(&ctx,IV,(const uint8_t*)"myy5981@outlook.com",19);
	r = sm4_gcm_decrypt(&ctx,d,c,MESSAGE_LEN,gmac,16);

	printf(" %1dd: ",r);
	hex_enc2stream(stdout,d,MESSAGE_LEN);

	printf("res: %d\n",memcmp(m,d,MESSAGE_LEN));

	#undef MESSAGE_LEN
}

const Task tasks[]={
	{.after=after,.before=before,.task=fun},
	{.after=NULL,.before=NULL,.task=test_sm4_gcm_dec}
};
int tasks_len=2;
