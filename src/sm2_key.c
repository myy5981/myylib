#include <myy/sm2.h>
#include <myy/random.h>

const BN_256 SM2_N		=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,7203DF6B,21C6052B,53BBF409,39D54123);
static const BN_256 SM2_N_SUB_1	=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,7203DF6B,21C6052B,53BBF409,39D54122);
static const BN_256 SM2_B		=	BN_256_INIT(28E9FA9E,9D9F5E34,4D5A9E4B,CF6509A7,F39789F5,15AB8F92,DDBCBD41,4D940E93);

// static SM2_POINT SM2_G={
// 	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
// 	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
// };

SM2_POINT SM2_G_MONT={
	.x=BN_256_INIT(91167a5e,e1c13b05,d6a1ed99,ac24c3c3,3e7981ed,dca6c050,61328990,f418029e),
	.y=BN_256_INIT(63cd65d4,81d735bd,8d4cfb06,6e2a48f8,c1f5e578,8d3295fa,c1354e59,3c2d0ddd),
};

void sm2_key_generate(SM2_PRI_KEY* prik){
	do{
		rand_bytes((uint8_t*)(prik->d),32);
	}while(bn_256_cmp(prik->d,SM2_N_SUB_1)>=0||bn_256_is_zero(prik->d));
	SM2_JPOINT pub;
	sm2_point_mul_mont(&pub,&SM2_G_MONT,prik->d);
	sm2_jpoint_to_point_mont(&(prik->Pub),&pub);
	sm2_point_from_mont(&(prik->Pub),&(prik->Pub));
}

void sm2_pub_key_generate(SM2_PUB_KEY* pubk, SM2_PRI_KEY* prik){
	bn_256_GFp_cpy_mont(pubk->x,prik->Pub.x);
	bn_256_GFp_cpy_mont(pubk->y,prik->Pub.y);
}

int sm2_pub_key_verify(SM2_PUB_KEY* pubk){
	BN_256_GFp t1,t2;
	if(sm2_point_is_zero(pubk)){
		return 1;
	}
	if(bn_256_cmp(pubk->x,SM2_P)>=0){
		return 1;
	}
	if(bn_256_cmp(pubk->y,SM2_P)>=0){
		return 1;
	}
	bn_256_GFp_sqr(t1,pubk->x);
	bn_256_GFp_mul(t1,t1,pubk->x);
	bn_256_GFp_dbl(t2,pubk->x);
	bn_256_GFp_add(t2,t2,pubk->x);
	bn_256_GFp_sub(t1,t1,t2);
	bn_256_GFp_add(t1,t1,SM2_B);
	bn_256_GFp_sqr(t2,pubk->y);
	if(bn_256_cmp(t1,t2)!=0){
		return 1;
	}
	SM2_JPOINT T;
	SM2_POINT t;
	sm2_point_to_mont(&t,pubk);
	sm2_point_mul_mont(&T,&t,SM2_N);
	if(sm2_jpoint_is_zero_mont(&T)){
		return 0;
	}
	return 1;
}
