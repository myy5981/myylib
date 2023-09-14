#include <myy/sm2.h>

int sm2_ecmqv_init(SM3_KDF_CTX* kdf, 
					const SM2_PRI_KEY* k, const SM2_PRI_KEY* rk,
					const SM2_PUB_KEY* opposite_k, const SM2_PUB_KEY* opposite_rk,
					const uint8_t Z_cli[32], const uint8_t Z_srv[32]){
	if(!sm2_point_is_on_cure(opposite_rk)){
		return 1;
	}
	BN_256 _x;
	BN_256_GFn t;
	_x[0]=0;_x[1]=0;_x[2]=rk->Pub.x[2]|0x8000000000000000ULL;_x[3]=rk->Pub.x[3];
	bn_256_GFn_mul(t,_x,rk->d);
	bn_256_GFn_add(t,t,k->d);
	_x[2]=opposite_rk->x[2]|0x8000000000000000ULL;_x[3]=opposite_rk->x[3];
	SM2_POINT tp;
	SM2_JPOINT tjp;
	sm2_point_to_mont(&tp,opposite_rk);
	sm2_point_mul_mont(&tjp,&tp,_x);
	sm2_point_to_mont(&tp,opposite_k);
	sm2_jpoint_add_point_mont(&tjp,&tjp,&tp);
	sm2_jpoint_to_point_mont(&tp,&tjp);
	sm2_point_mul_mont(&tjp,&tp,t);
	sm2_jpoint_to_point_mont(&tp,&tjp);
	#define U tp
	sm2_point_from_mont(&U,&tp);
	if(sm2_point_is_zero(&U)){
		return 1;
	}
	sm3_kdf_init(kdf);
	uint8_t buf[32];
	bn_256_to_bin(U.x,buf);
	sm3_kdf_init_update(kdf,buf,32);
	bn_256_to_bin(U.y,buf);
	sm3_kdf_init_update(kdf,buf,32);
	sm3_kdf_init_update(kdf,Z_cli,32);
	sm3_kdf_init_update(kdf,Z_srv,32);
	return 0;
}