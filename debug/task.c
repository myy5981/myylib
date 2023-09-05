#include <myy/bignum.h>
#include <myy/sm2.h>
#include <myy/hex.h>
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
	res1 = sm2_ecdhe_init(&ka,A,rA,&(B->Pub),&(rB->Pub),Za,Zb);
	res2 = sm2_ecdhe_init(&kb,B,rB,&(A->Pub),&(rA->Pub),Za,Zb);
}

void after(){
	printf("%d,%d\n",res1,res2);

	uint8_t buf[16];
	sm3_kdf_final(&ka,buf,16);
	hex_enc2stream(stdout,buf,16);
	sm3_kdf_final(&kb,buf,16);
	hex_enc2stream(stdout,buf,16);
}

const Task tasks[]={
	{.after=after,.before=before,.task=fun}
};
int tasks_len=1;
