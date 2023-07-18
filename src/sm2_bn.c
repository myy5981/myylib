#include <myy/sm2_bn.h>

const BN_256 SM2_P							=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF);
static const BN_512 SM2_P_512				=	BN_512_INIT(0,0,0,0,0,0,0,0,FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF);

// 2^256-p
static const BN_256 TWO_POWED_256_SUB_SM2_P	=	BN_256_INIT(00000001,00000000,00000000,00000000,00000000,FFFFFFFF,00000000,00000001);

// 2^512/p-2^256
static const BN_256 TWO_POWED_512_DIV_SM2_P	=	BN_256_INIT(00000001,00000001,00000001,00000001,00000002,00000002,00000002,00000003);

static const BN_256 K={0};

void bn_256_GFp_add(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	if(bn_256_adc(r,a,b)!=0){
		bn_256_add(r,r,TWO_POWED_256_SUB_SM2_P);
	}else if(bn_256_cmp(r,SM2_P)>=0){
		bn_256_sub(r,r,SM2_P);
	}
}

void bn_256_GFp_sub(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	if(bn_256_cmp(a,b)>=0){
		bn_256_sub(r,a,b);
	}else{
		bn_256_sub(r,SM2_P,b);
		bn_256_add(r,r,a);
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
void bn_256_GFp_mul(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
	BN_256_ptr q1,q2;
	BN_256 q3;
	BN_512 X,q2m,q3p,R;
	bn_256_mul(X,a,b);
	q1 = bn_512_h256(X);
	bn_256_mul(q2m,q1,TWO_POWED_512_DIV_SM2_P);
	q2 = bn_512_h256(q2m);
	bn_256_add(q3,q2,q1);
	bn_256_mul(q3p,q3,SM2_P);
	bn_288_sub(R,X,q3p);
	while(bn_288_cmp(R,SM2_P_512)>=0){
		bn_288_sub(R,R,SM2_P_512);
	}
	bn_256_cpy(r,bn_512_l256(R));
}

void bn_256_GFp_mont_redc(BN_256_GFp r, const BN_512 X){
	BN_256 m;
	BN_512 mP;
	bn_256_imul(m,bn_512_l256(X),K);
	bn_256_mul(mP,m,SM2_P);
	//bn_512_add(mp,mp,X)
	bn_256_cpy(r,bn_512_h256(mP));
	if(bn_256_cmp(r,SM2_P)>=0){
		bn_256_sub(r,r,SM2_P);
	}
}
