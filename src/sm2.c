#include <myy/sm2.h>

// static const BN_256 p  = {0xFFFFFFFEu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0x00000000u,0xFFFFFFFFu,0xFFFFFFFFu};
// static const BN_256 n  = {0xFFFFFFFEu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0x7203DF6Bu,0x21C6052Bu,0x53BBF409u,0x39D54123u};
// static const BN_256 Gx = {0x32C4AE2Cu,0x1F198119u,0x5F990446u,0x6A39C994u,0x8FE30BBFu,0xF2660BE1u,0x715A4589u,0x334C74C7u};
// static const BN_256 Gy = {0xBC3736A2u,0xF4F6779Cu,0x59BDCEE3u,0x6B692153u,0xD0A9877Cu,0xC62A4740u,0x02DF32E5u,0x2139F0A0u};

// static const BN_256 SM2_A	=	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFC);
// static const BN_256 SM2_B	=	BN_256_INIT(28E9FA9E,9D9F5E34,4D5A9E4B,CF6509A7,F39789F5,15AB8F92,DDBCBD41,4D940E93);

// static const JACOBIAN_POINT SM2_G={
// 	.x=BN_256_INIT(32C4AE2C,1F198119,5F990446,6A39C994,8FE30BBF,F2660BE1,715A4589,334C74C7),
// 	.y=BN_256_INIT(BC3736A2,F4F6779C,59BDCEE3,6B692153,D0A9877C,C62A4740,02DF32E5,2139F0A0),
// 	.z=BN_256_ONE
// };

void sm2_jacobian_to_point(SM2_POINT* r, const JACOBIAN_POINT* a){
	if(bn_256_is_zero(a->z)){
		bn_256_set_zero(r->x);
		bn_256_set_zero(r->y);
	}
	if(bn_256_is_one(a->z)){
		bn_256_cpy(r->x,a->x);
		bn_256_cpy(r->y,a->y);
	}
	BN_256_GFp z_inv;
	bn_256_GFp_inv(z_inv,a->z);
	bn_256_GFp_mul(r->y,a->y,z_inv);
	bn_256_GFp_sqr(z_inv,z_inv);
	bn_256_GFp_mul(r->y,r->y,z_inv);
	bn_256_GFp_mul(r->x,a->x,z_inv);
}

void sm2_point_dbl(JACOBIAN_POINT* r, const JACOBIAN_POINT* a){
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
	if(sm2_point_is_zero(a)){
		sm2_point_cpy(r,a);
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
	bn_256_GFp_mul(z3,la_2,z1);//插入算一下z3
	bn_256_GFp_sqr(tv,la_2);
	bn_256_GFp_mul(la_3,tv,la_2);
	bn_256_GFp_mul(la_3,la_3,y1);
	bn_256_GFp_mul(la_2,tv,x1);

	//x3
	bn_256_GFp_sqr(x3,la_1);
	bn_256_GFp_dbl(tv,la_2);
	bn_256_GFp_sub(x3,x3,tv);

	//y3
	bn_256_GFp_sub(y3,la_2,x3);
	bn_256_GFp_mul(y3,y3,la_1);
	bn_256_GFp_sub(y3,y3,la_3);
	//九次乘法，九次加减
	//而gmssl八次乘法九次加减，一次取半
	//这次是我败了
}
