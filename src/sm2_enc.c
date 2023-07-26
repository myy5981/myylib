#include <myy/sm2.h>
#include <myy/sm3_kdf.h>
#include <myy/random.h>
#include <string.h>

int sm2_encrypt(SM2_PUB_KEY* key,uint8_t* m,int len,uint8_t* c){
	BN_256 k;//=BN_256_INIT(59276E27,D506861A,16680F3A,D9C02DCC,EF3CC1FA,3CDBE4CE,6D54B80D,EAC1BC21);
	do{
		rand_bytes((uint8_t*)k,32);
	}while(bn_256_cmp(k,SM2_N)>=0||bn_256_is_zero(k));
	SM2_JPOINT _jp,*jp=&_jp;
	SM2_POINT _kg,*kg=&_kg;
	SM3_CTX _hctx,*hctx=&_hctx;
	SM3_KDF_CTX _kctx,*kctx=&_kctx;

	sm2_point_mul_mont(jp,&SM2_G_MONT,k);
	sm2_jpoint_to_point_mont(kg,jp);
	sm2_point_from_mont(kg,kg);
	int r = sm2_point_to_bin(kg,c,SM2_POINT_SERIALIZE_DEFAULT);// C1 is done

	sm2_point_to_mont(kg,key);
	sm2_point_mul_mont(jp,kg,k);
	sm2_jpoint_to_point_mont(kg,jp);
	sm2_point_from_mont(kg,kg);

	sm3_init(hctx);
	sm3_kdf_init(kctx);
	uint8_t buf[64];
	bn_256_to_bin(kg->x,buf);
	bn_256_to_bin(kg->y,buf+32);
	sm3_kdf_init_update(kctx,buf,64);
	sm3_update(hctx,buf,32);
	uint8_t* dst=c+r+32;
	int i;
	for (i = 0; i < (len>>5); i++) {
		sm3_kdf_next(kctx,buf);
		sm3_update(hctx,m,32);
		((uint64_t*)dst)[0]=(((uint64_t*)buf)[0])^(((uint64_t*)m)[0]);
		((uint64_t*)dst)[1]=(((uint64_t*)buf)[1])^(((uint64_t*)m)[1]);
		((uint64_t*)dst)[2]=(((uint64_t*)buf)[2])^(((uint64_t*)m)[2]);
		((uint64_t*)dst)[3]=(((uint64_t*)buf)[3])^(((uint64_t*)m)[3]);
		dst+=32;
		m+=32;
	}
	sm3_kdf_next(kctx,buf);
	for (i = 0; i < (len%32); i++){
		dst[i]=buf[i]^m[i];
	}
	sm3_update(hctx,m,i);
	sm3_update(hctx,buf+32,32);
	sm3_final(hctx,c+r);
	return len+r+32;
}

int sm2_decrypt(SM2_PRI_KEY* key,uint8_t* c,int len,uint8_t* m){
	SM2_POINT _kp,*kp=&_kp;
	SM2_JPOINT _t,*t=&_t;
	SM3_CTX _hctx,*hctx=&_hctx;
	SM3_KDF_CTX _kctx,*kctx=&_kctx;
	int r = sm2_point_from_bin_mont(kp,c);
	sm2_point_mul_mont(t,kp,key->d);
	sm2_jpoint_to_point_mont(kp,t);
	sm2_point_from_mont(kp,kp);

	sm3_init(hctx);
	sm3_kdf_init(kctx);
	uint8_t buf[64];
	bn_256_to_bin(kp->x,buf);
	bn_256_to_bin(kp->y,buf+32);
	sm3_kdf_init_update(kctx,buf,64);
	sm3_update(hctx,buf,32);

	uint8_t* dst=c+r+32;
	len=len-r-32;
	if(len<=0){
		return len;
	}
	int i;

	for (i = 0; i < (len>>5); i++) {
		sm3_kdf_next(kctx,buf);
		((uint64_t*)m)[0]=(((uint64_t*)buf)[0])^(((uint64_t*)dst)[0]);
		((uint64_t*)m)[1]=(((uint64_t*)buf)[1])^(((uint64_t*)dst)[1]);
		((uint64_t*)m)[2]=(((uint64_t*)buf)[2])^(((uint64_t*)dst)[2]);
		((uint64_t*)m)[3]=(((uint64_t*)buf)[3])^(((uint64_t*)dst)[3]);
		sm3_update(hctx,m,32);
		dst+=32;
		m+=32;
	}
	sm3_kdf_next(kctx,buf);
	for (i = 0; i < (len%32); i++){
		m[i]=buf[i]^dst[i];
	}
	sm3_update(hctx,m,i);
	sm3_update(hctx,buf+32,32);
	sm3_final(hctx,buf);
	if(bn_256_cmp((BN_256_ptr)buf,(BN_256_ptr)(c+r))!=0){
		return -1;
	}
	return len;
}
