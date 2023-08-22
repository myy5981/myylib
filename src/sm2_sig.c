#include <myy/sm2.h>
#include <myy/endian.h>
#include <myy/random.h>

static const char SM2_A_B_Gx_Gy[] = 
	"\xFF\xFF\xFF\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFC"
	"\x28\xE9\xFA\x9E\x9D\x9F\x5E\x34\x4D\x5A\x9E\x4B\xCF\x65\x09\xA7\xF3\x97\x89\xF5\x15\xAB\x8F\x92\xDD\xBC\xBD\x41\x4D\x94\x0E\x93"
	"\x32\xC4\xAE\x2C\x1F\x19\x81\x19\x5F\x99\x04\x46\x6A\x39\xC9\x94\x8F\xE3\x0B\xBF\xF2\x66\x0B\xE1\x71\x5A\x45\x89\x33\x4C\x74\xC7"
	"\xBC\x37\x36\xA2\xF4\xF6\x77\x9C\x59\xBD\xCE\xE3\x6B\x69\x21\x53\xD0\xA9\x87\x7C\xC6\x2A\x47\x40\x02\xDF\x32\xE5\x21\x39\xF0\xA0";

int sm2_id_generate(uint8_t hash[32], const SM2_PUB_KEY* key, const uint8_t* id, int len){
	if(len*8>65535){
		return 1;
	}
	SM3_CTX _ctx,*ctx=&_ctx;
	uint16_t ENTL = h2be_16(len*8);
	sm3_init(ctx);
	sm3_update(ctx,(uint8_t*)&ENTL,2);
	sm3_update(ctx,id,len);
	sm3_update(ctx,(uint8_t*)SM2_A_B_Gx_Gy,128);
	uint8_t buf[32];
	bn_256_to_bin((BN_256_ptr)(key->x),buf);
	sm3_update(ctx,buf,32);
	bn_256_to_bin((BN_256_ptr)(key->y),buf);
	sm3_update(ctx,buf,32);
	sm3_final(ctx,hash);
	return 0;
}

int sm2_sig_init(SM3_CTX* ctx, const SM2_PUB_KEY* key, const uint8_t* id, int len){
	if(len*8>65535){
		return 1;
	}
	uint16_t ENTL = h2be_16(len*8);
	sm3_init(ctx);
	sm3_update(ctx,(uint8_t*)&ENTL,2);
	sm3_update(ctx,id,len);
	sm3_update(ctx,(uint8_t*)SM2_A_B_Gx_Gy,128);
	uint8_t buf[32];
	bn_256_to_bin((BN_256_ptr)(key->x),buf);
	sm3_update(ctx,buf,32);
	bn_256_to_bin((BN_256_ptr)(key->y),buf);
	sm3_update(ctx,buf,32);
	sm3_final(ctx,buf);
	sm3_init(ctx);
	sm3_update(ctx,buf,32);
	return 0;
}

int sm2_sig_update(SM3_CTX* ctx, const uint8_t* message, size_t len){
	return sm3_update(ctx,message,len);
}

void sm2_sig_final(SM3_CTX* ctx, uint8_t hash[32]){
	sm3_final(ctx,hash);
}

void sm2_sig_generate(SM2_SIGNATURE* sig, const SM2_PRI_KEY_EXT* key, const uint8_t hash[32]){
	BN_256 e;
	BN_256 k;//=BN_256_INIT(59276E27,D506861A,16680F3A,D9C02DCC,EF3CC1FA,3CDBE4CE,6D54B80D,EAC1BC21);
	BN_256 r_k;
	SM2_JPOINT _jkg,*jkg=&_jkg;
	SM2_POINT _kg,*kg=&_kg;
	bn_256_from_bin(e,hash);
	retry:
	do{
		rand_bytes((uint8_t*)k,32);
	}while(bn_256_cmp(k,SM2_N)>=0||bn_256_is_zero(k));
	sm2_point_mul_mont(jkg,&SM2_G_MONT,k);
	sm2_jpoint_to_point_mont(kg,jkg);
	sm2_point_from_mont(kg,kg);
	bn_256_GFn_add(sig->r,e,kg->x);
	if(bn_256_is_zero(sig->r)){
		goto retry;
	}
	bn_256_add(r_k,sig->r,k);
	if(bn_256_cmp(r_k,SM2_N)==0){
		goto retry;
	}
	bn_256_GFn_mul(r_k,sig->r,key->key.d);
	bn_256_GFn_sub(r_k,k,r_k);
	bn_256_GFn_mul(sig->s,r_k,key->d1_inv);
	if(bn_256_is_zero(sig->s)){
		goto retry;
	}
}

int sm2_sig_verify(const SM2_SIGNATURE* sig, const SM2_PUB_KEY* key, const uint8_t hash[32]){
	if(bn_256_is_zero(sig->r)){
		return 1;
	}
	if(bn_256_cmp(sig->r,SM2_N)>=0){
		return 1;
	}
	if(bn_256_is_zero(sig->s)){
		return 1;
	}
	if(bn_256_cmp(sig->s,SM2_N)>=0){
		return 1;
	}
	BN_256 e,t,x1;
	bn_256_GFn_add(t,sig->r,sig->s);
	if(bn_256_is_zero(t)){
		return 1;
	}
	bn_256_from_bin(e,hash);
	SM2_JPOINT _jsg,*jsg=&_jsg;
	SM2_JPOINT _jtp,*jtp=&_jtp;
	sm2_point_mul_mont(jsg,&SM2_G_MONT,sig->s);
	SM2_POINT p_mont,sgtp;
	sm2_point_to_mont(&p_mont,key);
	sm2_point_mul_mont(jtp,&p_mont,t);
	sm2_jpoint_add_mont(jsg,jsg,jtp);
	sm2_jpoint_to_point_mont(&sgtp,jsg);
	bn_256_mont_to_GFp(x1,sgtp.x);
	bn_256_GFn_add(x1,x1,e);
	return bn_256_cmp(x1,sig->r)!=0;
}

void sm2_sig_to_bin(const SM2_SIGNATURE* sig, uint8_t out[64]){
	bn_256_to_bin(sig->r,out);
	bn_256_to_bin(sig->s,out+32);
}

void sm2_sig_from_bin(SM2_SIGNATURE* sig, const uint8_t in[64]){
	bn_256_from_bin(sig->r,in);
	bn_256_from_bin(sig->s,in+32);
}
