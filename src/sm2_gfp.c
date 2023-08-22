#include "conf.h"
#include <myy/sm2_bn.h>

const BN_256 SM2_P							=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF);

static const BN_256 SM2_U_ADD_1				=	BN_256_INIT(3FFFFFFF,BFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,C0000000,40000000,00000000);
// 2^256-p
static const BN_256 TWO_POWED_256_SUB_SM2_P	=	BN_256_INIT(00000001,00000000,00000000,00000000,00000000,FFFFFFFF,00000000,00000001);
// 2^512/p-2^256
static const BN_256 TWO_POWED_512_DIV_SM2_P	=	BN_256_INIT(00000001,00000001,00000001,00000001,00000002,00000002,00000002,00000003);
// R*R^-1-_P*P=1
static const BN_256 _P						=	BN_256_INIT(FFFFFFFC,00000001,FFFFFFFE,00000000,FFFFFFFF,00000001,00000000,00000001);
// 2^512%p
static const BN_256 RR_MOD_P				=	BN_256_INIT(00000004,00000002,00000001,00000001,00000002,FFFFFFFF,00000002,00000003);
// 2^256%p
#define R_MOD_P TWO_POWED_256_SUB_SM2_P

void bn_256_GFp_add(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	if(bn_256_adc(r,a,b)!=0){
		bn_256_inc(r,TWO_POWED_256_SUB_SM2_P);
	}else if(bn_256_cmp(r,SM2_P)>=0){
		bn_256_sub(r,r,SM2_P);
	}
}

void bn_256_GFp_sub(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	if(bn_256_cmp(a,b)>=0){
		bn_256_sub(r,a,b);
	}else{
		BN_256 t;
		bn_256_sub(t,SM2_P,b);
		bn_256_add(r,t,a);
	}
}

void bn_256_GFp_neg(BN_256_GFp r, const BN_256_GFp a){
	if(bn_256_is_zero(a)){
		if(r!=a){
			bn_256_cpy(r,a);
		}
	}else{
		bn_256_sub(r,SM2_P,a);
	}
}

// Barrett Reduction
// 有问题
void bn_256_GFp_mul(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	// BN_256_GFp _a,_b;
	// if(bn_256_cmp(a,SM2_P)>=0){
	// 	bn_256_sub(_a,a,SM2_P);
	// }else{
	// 	bn_256_cpy(_a,a);
	// }
	// if(bn_256_cmp(b,SM2_P)>=0){
	// 	bn_256_sub(_b,a,SM2_P);
	// }
	// }else{
	// 	bn_256_cpy(_b,b);
	// }
	BN_256_ptr q1,q2;
	BN_256 q3;
	BN_512 X,q2m,q3p,R;
	bn_256_mul(X,a,b);
	q1 = bn_512_h256(X);
	bn_256_mul(q2m,q1,TWO_POWED_512_DIV_SM2_P);
	q2 = bn_512_h256(q2m);
	/*
		问题在于bn_256_add(q3,q2,q1)处可能发生进位，即q3超过了256位，所以仅当a*b超过P*P很多时会发生
		具体阈值不明确（懒得算），但是该函数传入的ab参数应该在GF(P)中，故暂且不做修改
	*/
	bn_256_add(q3,q2,q1);
	bn_256_mul(q3p,q3,SM2_P);
	__bn_288_sub(R,X,q3p);
	while(__bn_288_cmp_256(R,SM2_P)>=0){
		__bn_288_sbb_256(R,SM2_P);
	}
	bn_256_cpy(r,bn_512_l256(R));
}

void bn_256_GFp_exp(BN_256_GFp r, const BN_256_GFp a, const BN_256 e){
	BN_256_GFp_Mont A;
	bn_256_GFp_to_mont(A,a);
	BN_256_GFp_Mont R=BN_256_ONE;
	BN_512 T;
	BN_UNIT n;
	bn_256_GFp_to_mont(R,R);
	for (int i = BN_256_LEN-1; i >= 1; --i){
		n=e[i];
		for (int j = 0; j < BN_UNIT_LEN; j++){
			if((n>>j)&1){
				bn_256_mul(T,R,A);
				bn_256_GFp_mont_redc(R,T);
			}
			bn_256_mul(T,A,A);
			bn_256_GFp_mont_redc(A,T);
		}
	}
	n=e[0];
	while(n){
		if(n&(BN_UNIT)1){
			bn_256_mul(T,R,A);
			bn_256_GFp_mont_redc(R,T);
		}
		n>>=1;
		bn_256_mul(T,A,A);
		bn_256_GFp_mont_redc(A,T);
	}
	bn_256_mont_to_GFp(r,R);
}

void bn_256_GFp_new_inv(BN_256_GFp r, const BN_256_GFp a){
	BN_256_GFn x1;
	bn_256_cpy(x1,a);
	BN_256_GFn y1=BN_256_ONE;
	BN_256_GFn x2=BN_256_SM2_P;
	BN_256_GFn y2=BN_256_ZERO;

	while(1){
		while(!bn_256_is_odd(x1)){
			bn_256_rshift(x1,1);
			if(bn_256_is_odd(y1)){
				if(bn_256_adc(y1,y1,SM2_P)){
					bn_256_rshift(y1,1);
					y1[0]|=(1ULL<<(sizeof(BN_UNIT)*8-1));
				}else{
					bn_256_rshift(y1,1);
				}
			}else{
				bn_256_rshift(y1,1);
			}
		}
		while(!bn_256_is_odd(x2)){
			bn_256_rshift(x2,1);
			if(bn_256_is_odd(y2)){
				if(bn_256_adc(y2,y2,SM2_P)){
					bn_256_rshift(y2,1);
					y2[0]|=(1ULL<<(sizeof(BN_UNIT)*8-1));
				}else{
					bn_256_rshift(y2,1);
				}
			}else{
				bn_256_rshift(y2,1);
			}
		}
		if(bn_256_is_one(x1)){
			bn_256_cpy(r,y1);
			return;
		}
		if(bn_256_is_one(x2)){
			bn_256_cpy(r,y2);
			return;
		}
		if(bn_256_cmp(x1,x2)>0){
			bn_256_sub(x1,x1,x2);
			bn_256_GFp_sub(y1,y1,y2);
		}else{
			bn_256_sub(x2,x2,x1);
			bn_256_GFp_sub(y2,y2,y1);
		}
		if(bn_256_is_one(x1)){
			bn_256_cpy(r,y1);
			return;
		}
		if(bn_256_is_one(x2)){
			bn_256_cpy(r,y2);
			return;
		}
	}
}

#define __mont_sqr(R,A) bn_256_mul(T,A,A);bn_256_GFp_mont_redc(R,T)
#define __mont_mul(R,A,B) bn_256_mul(T,A,B);bn_256_GFp_mont_redc(R,T)
void bn_256_GFp_inv(BN_256_GFp r, const BN_256_GFp a){
	//r=a^(P-2)
	BN_256_GFp_Mont A;BN_512 T;int i;
	BN_256_GFp_Mont a1,a2,a3,a4,a5;
	bn_256_mul(T,a,RR_MOD_P);
	bn_256_GFp_mont_redc(A,T);
	__mont_sqr(a1,A);
	__mont_mul(a2,a1,A);
	__mont_sqr(a3,a2);
	__mont_sqr(a3,a3);
	__mont_mul(a3,a3,a2);
	__mont_sqr(a4,a3);
	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,a3);
	__mont_sqr(a5,a4);
	for (i = 1; i < 8; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a4);
	for (i = 0; i < 8; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a4);
	for (i = 0; i < 4; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a3);
	__mont_sqr(a5,a5);
	__mont_sqr(a5,a5);
	__mont_mul(a5,a5,a2);
	__mont_sqr(a5,a5);
	__mont_mul(a5,a5,A);
	__mont_sqr(a4,a5);
	__mont_mul(a3,a4,a1);
	__mont_sqr(a5,a4);
	for (i = 1; i< 31; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a4,a5,a4);
	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,A);
	__mont_mul(a3,a4,a2);
	for (i = 0; i < 33; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(r,a4,a5);
	bn_256_mont_to_GFp(r,r);
}

int bn_256_GFp_sqrt(BN_256_GFp r, const BN_256_GFp a){
	//r = a^(SM2_U+1)
	BN_256_GFp R;
	bn_256_GFp_exp(R,a,SM2_U_ADD_1);
	BN_256 t;
	bn_256_GFp_sqr(t,R);
	if(bn_256_GFp_cmp(t,a)==0){
		bn_256_cpy(r,&R);
		return 0;
	}
	return 1;
}

void bn_256_GFp_mont_redc(BN_256_GFp_Mont r, const BN_512 X){
	if(bn_256_GFp_is_zero(bn_512_l256(X))){
		if(bn_256_cmp(bn_512_h256(X),SM2_P)>=0){
			bn_256_sub(r,bn_512_h256(X),SM2_P);
		}else{
			bn_256_cpy(r,bn_512_h256(X));
		}
		return;
	}
	BN_256 m;
	BN_512 mP;
	bn_256_imul(m,bn_512_l256(X),_P);
	bn_256_mul(mP,m,SM2_P);
	if(__bn_512_h256_adc(r,mP,X)!=0){
		bn_256_inc(r,TWO_POWED_256_SUB_SM2_P);
	}else if(bn_256_cmp(r,SM2_P)>=0){
		bn_256_sub(r,r,SM2_P);
	}
}

void bn_256_mont_to_GFp(BN_256_GFp r, const BN_256_GFp_Mont X){
	if(bn_256_GFp_is_zero_mont(X)){
		bn_256_GFp_set_zero(r);
		return;
	}
	BN_256 m;
	BN_512 mP;
	bn_256_imul(m,X,_P);
	bn_256_mul(mP,m,SM2_P);
	int f=bn_256_inc1(bn_512_h256(mP));
	if(f!=0){
		bn_256_add(r,bn_512_h256(mP),TWO_POWED_256_SUB_SM2_P);
	}else if(bn_256_cmp(bn_512_h256(mP),SM2_P)>=0){
		bn_256_sub(r,bn_512_h256(mP),SM2_P);
	}else{
		bn_256_cpy(r,bn_512_h256(mP));
	}
}

void bn_256_GFp_to_mont(BN_256_GFp_Mont r,const BN_256_GFp X){
	BN_512 XRR={0};
	bn_256_mul(XRR,X,RR_MOD_P);
	bn_256_GFp_mont_redc(r,XRR);
}

void bn_256_GFp_mul_mont(BN_256_GFp_Mont r, const BN_256_GFp_Mont a, const BN_256_GFp_Mont b){
	BN_512 ab;
	bn_256_mul(ab,a,b);
	bn_256_GFp_mont_redc(r,ab);
}

void bn_256_GFp_inv_mont(BN_256_GFp_Mont r, const BN_256_GFp_Mont a){
	//r=a^(P-2)
	BN_512 T;int i;
	BN_256_GFp_Mont a1,a2,a3,a4,a5;
	__mont_sqr(a1,a);
	__mont_mul(a2,a1,a);
	__mont_sqr(a3,a2);
	__mont_sqr(a3,a3);
	__mont_mul(a3,a3,a2);
	__mont_sqr(a4,a3);
	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,a3);
	__mont_sqr(a5,a4);
	for (i = 1; i < 8; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a4);
	for (i = 0; i < 8; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a4);
	for (i = 0; i < 4; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a5,a5,a3);
	__mont_sqr(a5,a5);
	__mont_sqr(a5,a5);
	__mont_mul(a5,a5,a2);
	__mont_sqr(a5,a5);
	__mont_mul(a5,a5,a);
	__mont_sqr(a4,a5);
	__mont_mul(a3,a4,a1);
	__mont_sqr(a5,a4);
	for (i = 1; i< 31; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a4,a5,a4);
	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,a);
	__mont_mul(a3,a4,a2);
	for (i = 0; i < 33; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(a2,a5,a3);
	__mont_mul(a3,a2,a3);
	__mont_mul(a4,a2,a4);
	for (i = 0; i < 32; i++){
		__mont_sqr(a5,a5);
	}
	__mont_mul(r,a4,a5);
}

//(3FFFFFFF,BFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,C0000000,40000000,00000000)
//0011 1111 FFFFFF
int bn_256_GFp_sqrt_mont(BN_256_GFp_Mont r, const BN_256_GFp_Mont a){
	if(bn_256_GFp_is_zero_mont(a)){
		bn_256_GFp_set_zero_mont(r);
		return 0;
	}
	BN_512 T;
	BN_256_GFp_Mont R;
	BN_256_GFp_Mont a1,a2,a3,a4,a5;
	__mont_sqr(a1,a);		//a1=a^(10)
	__mont_mul(a2,a1,a);	//a2=a^(11)
	__mont_sqr(R,a2);
	__mont_sqr(R,R);		//r=a^(1100)
	__mont_mul(a3,R,a2);	//a3=a^(1111)
	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a3);		//r=a^(111111)=a^(3F)
	__mont_sqr(a4,a3);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,a3);	//a4=a^(11111111)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a4);		//r=a^(3FFF)
	__mont_sqr(a5,a4);	__mont_sqr(a5,a5);	__mont_sqr(a5,a5);	__mont_sqr(a5,a5);
	__mont_sqr(a5,a5);	__mont_sqr(a5,a5);	__mont_sqr(a5,a5);	__mont_sqr(a5,a5);
	__mont_mul(a5,a5,a4);	//a5=a^(1111111111111111)=a^(FFFF)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a5);		//r=a^(3FFFFFFF)
	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a1);
	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a2);		//r=a^(3FFFFFFF B)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a3);		//r=a^(3FFFFFFF BF)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a4);		//r=a^(3FFFFFFF BFFF)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a5);		//r=a^(3FFFFFFF BFFFFFFF)
	__mont_sqr(a4,a5);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);
	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);	__mont_sqr(a4,a4);
	__mont_mul(a4,a4,a5);	//a4=a^(FFFFFFFF)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a4);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a4);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a4);	//r=a^(3FFFFFFF BFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF)
	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a2);	//r=a^(3FFFFFFF BFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 11)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_mul(R,R,a);	//r=a^(3FFFFFFF BFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF C0000000 01)
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);	__mont_sqr(R,R);
	__mont_sqr(R,R);	__mont_sqr(R,R);//共298次乘法

	__mont_sqr(a1,R);
	if(bn_256_GFp_eql_mont(a1,a)){
		bn_256_cpy(r,&R);
		return 0;
	}
	return 1;
}
#undef __mont_spr
#undef __mont_mul

int bn_256_GFp_is_one_mont(const BN_256_GFp_Mont a){
	return bn_256_cmp(a,R_MOD_P)==0;
}

void bn_256_GFp_set_one_mont(BN_256_GFp_Mont a){
	bn_256_cpy(a,R_MOD_P);
}