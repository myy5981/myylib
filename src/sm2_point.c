#include <myy/sm2.h>

// static const BN_256 p  = {0xFFFFFFFEu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0x00000000u,0xFFFFFFFFu,0xFFFFFFFFu};
// static const BN_256 n  = {0xFFFFFFFEu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0x7203DF6Bu,0x21C6052Bu,0x53BBF409u,0x39D54123u};
// static const BN_256 Gx = {0x32C4AE2Cu,0x1F198119u,0x5F990446u,0x6A39C994u,0x8FE30BBFu,0xF2660BE1u,0x715A4589u,0x334C74C7u};
// static const BN_256 Gy = {0xBC3736A2u,0xF4F6779Cu,0x59BDCEE3u,0x6B692153u,0xD0A9877Cu,0xC62A4740u,0x02DF32E5u,0x2139F0A0u};

// static const BN_256 SM2_A	=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFC);
// static const BN_256 SM2_B	=	BN_256_INIT(28E9FA9E,9D9F5E34,4D5A9E4B,CF6509A7,F39789F5,15AB8F92,DDBCBD41,4D940E93);

// static const SM2_JPOINT SM2_G={
// 	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
// 	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
// 	.z=BN_256_ONE
// };

void sm2_jpoint_to_point(SM2_POINT* r, const SM2_JPOINT* a){
	if(bn_256_is_zero(a->z)){
		bn_256_set_zero(r->x);
		bn_256_set_zero(r->y);
		return;
	}
	if(bn_256_is_one(a->z)){
		bn_256_cpy(r->x,a->x);
		bn_256_cpy(r->y,a->y);
		return;
	}
	BN_256_GFp z_inv;
	bn_256_GFp_inv(z_inv,a->z);
	bn_256_GFp_mul(r->y,a->y,z_inv);
	bn_256_GFp_sqr(z_inv,z_inv);
	bn_256_GFp_mul(r->y,r->y,z_inv);
	bn_256_GFp_mul(r->x,a->x,z_inv);
}

void sm2_point_to_jpoint(SM2_JPOINT* r, const SM2_POINT* a){
	bn_256_cpy(r->x,a->x);
	bn_256_cpy(r->y,a->y);
	bn_256_set_word(r->z,1);
}

void sm2_jpoint_dbl(SM2_JPOINT* r, const SM2_JPOINT* a){
	BN_256_GFp_ptr x1=(BN_256_GFp_ptr)(a->x);
	BN_256_GFp_ptr y1=(BN_256_GFp_ptr)(a->y);
	BN_256_GFp_ptr z1=(BN_256_GFp_ptr)(a->z);
	BN_256_GFp la_1=BN_256_ZERO;
	BN_256_GFp la_2=BN_256_ZERO;
	BN_256_GFp la_3=BN_256_ZERO;
	BN_256_GFp tv=BN_256_ZERO;

	BN_256_GFp_ptr x3=r->x;
	BN_256_GFp_ptr y3=r->y;
	BN_256_GFp_ptr z3=r->z;
	if(sm2_jpoint_is_zero(a)){
		sm2_jpoint_cpy(r,a);
		return;
	}

	// SM_A = SM_P - 3
	// A*(z^2)=(P-3)*(z^2)=-3(z^2)

	//lambda_1=3(x1-z1^2)(x1+z1^2)
	bn_256_GFp_sqr(la_1,z1);
	bn_256_GFp_add(tv,x1,la_1);
	bn_256_GFp_sub(la_1,x1,la_1);
	bn_256_GFp_mul(la_1,la_1,tv);
	bn_256_GFp_dbl(tv,la_1);
	bn_256_GFp_add(la_1,tv,la_1);

	//lambda_2 and lambda_3
	bn_256_GFp_dbl(la_2,y1);
	bn_256_GFp_mul(z3,la_2,z1);//顺便算一下z3
	bn_256_GFp_sqr(tv,la_2);
	bn_256_GFp_mul(la_3,tv,la_2);//与gmssl差在这里，可以通过平方然后取半得到lambda_3
	bn_256_GFp_mul(la_3,la_3,y1);//而这里用的是4y1^2 * 2y1 * y1两次乘法
	bn_256_GFp_mul(la_2,tv,x1);//不想为这个单独写个取半的api，暂且这样吧，差别不大

	//x3
	bn_256_GFp_sqr(x3,la_1);
	bn_256_GFp_dbl(tv,la_2);
	bn_256_GFp_sub(x3,x3,tv);

	//y3
	bn_256_GFp_sub(y3,la_2,x3);
	bn_256_GFp_mul(y3,y3,la_1);
	bn_256_GFp_sub(y3,y3,la_3);
	//九次乘法，九次加减
	//而gmssl八次乘法九次加减，一次取半（取半为一次位移和至多一次加法）
	//这次是我败了2333
}

// 对不起，这个直接🇰🇷了，来源于gmssl:src/sm2_alg.c/sm2_jacobian_point_add
void sm2_jpoint_add_point(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_POINT* b){

	if(sm2_jpoint_is_zero(a)){
		sm2_point_to_jpoint(r,b);
		return;
	}

	BN_256_GFp_ptr x1=(BN_256_GFp_ptr)(a->x);
	BN_256_GFp_ptr y1=(BN_256_GFp_ptr)(a->y);
	BN_256_GFp_ptr z1=(BN_256_GFp_ptr)(a->z);
	BN_256_GFp_ptr x2=(BN_256_GFp_ptr)(b->x);
	BN_256_GFp_ptr y2=(BN_256_GFp_ptr)(b->y);
	BN_256_GFp_ptr x3=(BN_256_GFp_ptr)(r->x);
	BN_256_GFp_ptr y3=(BN_256_GFp_ptr)(r->y);
	BN_256_GFp_ptr z3=(BN_256_GFp_ptr)(r->z);

	BN_256_GFp t1,t2,t3,t4;

	bn_256_GFp_sqr(t1,z1);
	bn_256_GFp_mul(t2,t1,z1);
	bn_256_GFp_mul(t1,t1,x2);
	bn_256_GFp_mul(t2,t2,y2);

	bn_256_GFp_sub(t1,t1,x1);
	bn_256_GFp_sub(t2,t2,y1);
	if(bn_256_is_zero(t1)){
		if(bn_256_is_zero(t2)){
			sm2_point_to_jpoint(r,b);
			sm2_jpoint_dbl(r,r);
			return;
		}else{
			bn_256_set_word(r->x,1);
			bn_256_set_word(r->y,1);
			bn_256_set_zero(r->z);
			return;
		}
	}
	bn_256_GFp_mul(z3,z1,t1);
	bn_256_GFp_sqr(t3,t1);
	bn_256_GFp_mul(t4,t3,t1);
    bn_256_GFp_mul(t3, t3, x1);
    bn_256_GFp_dbl(t1, t3);
    bn_256_GFp_sqr(x3, t2);
    bn_256_GFp_sub(x3, x3, t1);
    bn_256_GFp_sub(x3, x3, t4);
    bn_256_GFp_sub(t3, t3, x3);
    bn_256_GFp_mul(t3, t3, t2);
    bn_256_GFp_mul(t4, t4, y1);
    bn_256_GFp_sub(y3, t3, t4);
}

void sm2_point_mul(SM2_JPOINT* r, const SM2_POINT* a, const BN_256 k){
	int i,j;
	BN_UNIT e;
	bn_256_set_word(r->x,1);
	bn_256_set_word(r->y,1);
	bn_256_set_zero(r->z);
	for(i=0;i<BN_256_LEN;i++){
		e=k[i];
		for(j=BN_UNIT_LEN-1;j>=0;--j){
			sm2_jpoint_dbl(r,r);
			if((e>>j)&((BN_UNIT)1)){
				sm2_jpoint_add_point(r,r,a);
			}
		}
	}
}

/* 蒙哥马利实现 */
void sm2_jpoint_to_point_mont(SM2_POINT* r, const SM2_JPOINT* a){
	if(bn_256_GFp_is_zero_mont(a->z)){
		bn_256_GFp_set_zero_mont(r->x);
		bn_256_GFp_set_zero_mont(r->y);
		return;
	}
	if(bn_256_GFp_is_one_mont(a->z)){
		bn_256_cpy(r->x,a->x);
		bn_256_cpy(r->y,a->y);
		return;
	}
	BN_256_GFp_Mont z_inv;
	bn_256_GFp_inv_mont(z_inv,a->z);
	bn_256_GFp_mul_mont(r->y,a->y,z_inv);
	bn_256_GFp_sqr_mont(z_inv,z_inv);
	bn_256_GFp_mul_mont(r->y,r->y,z_inv);
	bn_256_GFp_mul_mont(r->x,a->x,z_inv);
}

void sm2_point_to_jpoint_mont(SM2_JPOINT* r, const SM2_POINT* a){
	bn_256_cpy(r->x,a->x);
	bn_256_cpy(r->y,a->y);
	bn_256_GFp_set_one_mont(r->z);
}

void sm2_jpoint_dbl_mont(SM2_JPOINT* r, const SM2_JPOINT* a){
	BN_256_GFp_Mont_ptr x1=(BN_256_GFp_Mont_ptr)(a->x);
	BN_256_GFp_Mont_ptr y1=(BN_256_GFp_Mont_ptr)(a->y);
	BN_256_GFp_Mont_ptr z1=(BN_256_GFp_Mont_ptr)(a->z);
	BN_256_GFp_Mont la_1=BN_256_ZERO;
	BN_256_GFp_Mont la_2=BN_256_ZERO;
	BN_256_GFp_Mont la_3=BN_256_ZERO;
	BN_256_GFp_Mont tv=BN_256_ZERO;

	BN_256_GFp_Mont_ptr x3=r->x;
	BN_256_GFp_Mont_ptr y3=r->y;
	BN_256_GFp_Mont_ptr z3=r->z;
	if(sm2_jpoint_is_zero_mont(a)){
		sm2_jpoint_cpy(r,a);
		return;
	}

	// SM_A = SM_P - 3
	// A*(z^2)=(P-3)*(z^2)=-3(z^2)

	//lambda_1=3(x1-z1^2)(x1+z1^2)
	bn_256_GFp_sqr_mont(la_1,z1);
	bn_256_GFp_add_mont(tv,x1,la_1);
	bn_256_GFp_sub_mont(la_1,x1,la_1);
	bn_256_GFp_mul_mont(la_1,la_1,tv);
	bn_256_GFp_dbl_mont(tv,la_1);
	bn_256_GFp_add_mont(la_1,tv,la_1);

	//lambda_2 and_mont lambda_3
	bn_256_GFp_dbl_mont(la_2,y1);
	bn_256_GFp_mul_mont(z3,la_2,z1);//顺便算一下z3
	bn_256_GFp_sqr_mont(tv,la_2);
	bn_256_GFp_mul_mont(la_3,tv,la_2);//与gmssl差在这里，可以通过平方然后取半得到lambda_3
	bn_256_GFp_mul_mont(la_3,la_3,y1);//而这里用的是4y1^2 * 2y1 * y1两次乘法
	bn_256_GFp_mul_mont(la_2,tv,x1);//不想为这个单独写个取半的api，暂且这样吧，差别不大

	//x3
	bn_256_GFp_sqr_mont(x3,la_1);
	bn_256_GFp_dbl_mont(tv,la_2);
	bn_256_GFp_sub_mont(x3,x3,tv);

	//y3
	bn_256_GFp_sub_mont(y3,la_2,x3);
	bn_256_GFp_mul_mont(y3,y3,la_1);
	bn_256_GFp_sub_mont(y3,y3,la_3);
	//九次乘法，九次加减
	//而gmssl八次乘法九次加减，一次取半（取半为一次位移和至多一次加法）
	//这次是我败了2333
}

void sm2_jpoint_add_point_mont(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_POINT* b){

	if(sm2_jpoint_is_zero_mont(a)){
		sm2_point_to_jpoint_mont(r,b);
		return;
	}

	BN_256_GFp_Mont_ptr x1=(BN_256_GFp_Mont_ptr)(a->x);
	BN_256_GFp_Mont_ptr y1=(BN_256_GFp_Mont_ptr)(a->y);
	BN_256_GFp_Mont_ptr z1=(BN_256_GFp_Mont_ptr)(a->z);
	BN_256_GFp_Mont_ptr x2=(BN_256_GFp_Mont_ptr)(b->x);
	BN_256_GFp_Mont_ptr y2=(BN_256_GFp_Mont_ptr)(b->y);
	BN_256_GFp_Mont_ptr x3=(BN_256_GFp_Mont_ptr)(r->x);
	BN_256_GFp_Mont_ptr y3=(BN_256_GFp_Mont_ptr)(r->y);
	BN_256_GFp_Mont_ptr z3=(BN_256_GFp_Mont_ptr)(r->z);

	BN_256_GFp_Mont t1,t2,t3,t4;

	bn_256_GFp_sqr_mont(t1,z1);
	bn_256_GFp_mul_mont(t2,t1,z1);
	bn_256_GFp_mul_mont(t1,t1,x2);
	bn_256_GFp_mul_mont(t2,t2,y2);

	bn_256_GFp_sub_mont(t1,t1,x1);
	bn_256_GFp_sub_mont(t2,t2,y1);
	if(bn_256_GFp_is_zero_mont(t1)){
		if(bn_256_GFp_is_zero_mont(t2)){
			sm2_point_to_jpoint_mont(r,b);
			sm2_jpoint_dbl_mont(r,r);
			return;
		}else{
			bn_256_GFp_set_one_mont(r->x);
			bn_256_GFp_set_one_mont(r->y);
			bn_256_GFp_set_zero_mont(r->z);
			return;
		}
	}
	bn_256_GFp_mul_mont(z3,z1,t1);
	bn_256_GFp_sqr_mont(t3,t1);
	bn_256_GFp_mul_mont(t4,t3,t1);
    bn_256_GFp_mul_mont(t3, t3, x1);
    bn_256_GFp_dbl_mont(t1, t3);
    bn_256_GFp_sqr_mont(x3, t2);
    bn_256_GFp_sub_mont(x3, x3, t1);
    bn_256_GFp_sub_mont(x3, x3, t4);
    bn_256_GFp_sub_mont(t3, t3, x3);
    bn_256_GFp_mul_mont(t3, t3, t2);
    bn_256_GFp_mul_mont(t4, t4, y1);
    bn_256_GFp_sub_mont(y3, t3, t4);
}

void sm2_point_mul_mont(SM2_JPOINT* r, const SM2_POINT* a, const BN_256 k){
	int i,j;
	BN_UNIT e;
	bn_256_GFp_set_one_mont(r->x);
	bn_256_GFp_set_one_mont(r->y);
	bn_256_GFp_set_zero_mont(r->z);
	for(i=0;i<BN_256_LEN;i++){
		e=k[i];
		for(j=BN_UNIT_LEN-1;j>=0;--j){
			sm2_jpoint_dbl_mont(r,r);
			if((e>>j)&((BN_UNIT)1)){
				sm2_jpoint_add_point_mont(r,r,a);
			}
		}
	}
}

void sm2_jpoint_to_mont(SM2_JPOINT* r,SM2_JPOINT* a){
	bn_256_GFp_to_mont(r->x,a->x);
	bn_256_GFp_to_mont(r->y,a->y);
	bn_256_GFp_to_mont(r->z,a->z);
}

void sm2_jpoint_from_mont(SM2_JPOINT* r,SM2_JPOINT* a){
	bn_256_mont_to_GFp(r->x,a->x);
	bn_256_mont_to_GFp(r->y,a->y);
	bn_256_mont_to_GFp(r->z,a->z);
}

void sm2_point_to_mont(SM2_POINT* r,SM2_POINT* a){
	bn_256_GFp_to_mont(r->x,a->x);
	bn_256_GFp_to_mont(r->y,a->y);
}

void sm2_point_from_mont(SM2_POINT* r,SM2_POINT* a){
	bn_256_mont_to_GFp(r->x,a->x);
	bn_256_mont_to_GFp(r->y,a->y);
}

int sm2_point_to_bin(SM2_POINT* r, uint8_t* dst, int flag){
	switch (flag)
	{
	case SM2_POINT_SERIALIZE_DEFAULT:
		dst[0]=0x04;
		bn_256_to_bin(r->x,dst+1);
		bn_256_to_bin(r->y,dst+33);
		return 65;
	case SM2_POINT_SERIALIZE_COMPRESS:
		if(bn_256_is_odd(r->y)){
			dst[0]=0x03;
		}else{
			dst[0]=0x02;
		}
		bn_256_to_bin(r->x,dst+1);
		return 33;
	case SM2_POINT_SERIALIZE_MIX:
		if(bn_256_is_odd(r->y)){
			dst[0]=0x07;
		}else{
			dst[0]=0x06;
		}
		bn_256_to_bin(r->x,dst+1);
		bn_256_to_bin(r->y,dst+33);
		return 65;
	default:
		return 0;
	}
}

int sm2_point_to_bin_mont(SM2_POINT* r, uint8_t* dst, int flag){
	SM2_POINT T;
	sm2_point_from_mont(&T,r);
	return sm2_point_to_bin(&T,dst,flag);
}

int sm2_point_from_bin(SM2_POINT* r, uint8_t* dst){
	if(dst[0]!=0x04){
		return 1;
	}
	bn_256_from_bin(r->x,dst+1);
	bn_256_from_bin(r->y,dst+33);
	return 65;
}

int sm2_point_from_bin_mont(SM2_POINT* r, uint8_t* dst){
	if(dst[0]!=0x04){
		return 1;
	}
	bn_256_from_bin(r->x,dst+1);
	bn_256_from_bin(r->y,dst+33);
	sm2_point_to_mont(r,r);
	return 65;
}
