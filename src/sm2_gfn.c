#include "conf.h"
#include <myy/sm2_bn.h>

const BN_256 SM2_N							=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,7203DF6B,21C6052B,53BBF409,39D54123);
static const BN_256 TWO_POWED_256_SUB_SM2_N	=	BN_256_INIT(00000001,00000000,00000000,00000000,8DFC2094,DE39FAD4,AC440BF6,C62ABEDD);
static const BN_256 TWO_POWED_512_DIV_SM2_N	=	BN_256_INIT(00000001,00000001,00000001,00000001,8DFC2096,FA323C01,12AC6361,F15149A0);

void bn_256_GFn_add (BN_256_GFn r, const BN_256_GFn a, const BN_256_GFn b){
	if(bn_256_adc(r,a,b)!=0){
		bn_256_inc(r,TWO_POWED_256_SUB_SM2_N);
	}else if(bn_256_cmp(r,SM2_N)>=0){
		bn_256_sub(r,r,SM2_N);
	}
}

void bn_256_GFn_sub (BN_256_GFn r, const BN_256_GFn a, const BN_256_GFn b){
	if(bn_256_cmp(a,b)>=0){
		bn_256_sub(r,a,b);
	}else{
		BN_256 t;
		bn_256_sub(t,SM2_N,b);
		bn_256_add(r,t,a);
	}
}

void bn_256_GFn_mul (BN_256_GFn r, const BN_256_GFn a, const BN_256_GFn b){
	BN_256_ptr q1,q2;
	BN_256 q3;
	BN_512 X,q2m,q3p,R;
	bn_256_mul(X,a,b);
	q1 = bn_512_h256(X);
	bn_256_mul(q2m,q1,TWO_POWED_512_DIV_SM2_N);
	q2 = bn_512_h256(q2m);
	bn_256_add(q3,q2,q1);
	bn_256_mul(q3p,q3,SM2_N);
	__bn_288_sub(R,X,q3p);
	while(__bn_288_cmp_256(R,SM2_N)>=0){
		__bn_288_sbb_256(R,SM2_N);
	}
	bn_256_cpy(r,bn_512_l256(R));
}

void bn_256_GFn_inv (BN_256_GFn r, const BN_256_GFn a){
	BN_256_GFn x1;
	bn_256_cpy(x1,a);
	BN_256_GFn y1=BN_256_ONE;
	BN_256_GFn x2=BN_256_SM2_N;
	BN_256_GFn y2=BN_256_ZERO;

	while(1){
		while(!bn_256_is_odd(x1)){
			bn_256_rshift(x1,1);
			if(bn_256_is_odd(y1)){
				if(bn_256_adc(y1,y1,SM2_N)){
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
				if(bn_256_adc(y2,y2,SM2_N)){
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
			bn_256_GFn_sub(y1,y1,y2);
		}else{
			bn_256_sub(x2,x2,x1);
			bn_256_GFn_sub(y2,y2,y1);
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

