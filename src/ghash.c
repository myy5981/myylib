#include <myy/ghash.h>
#include <myy/endian.h>

void bn_128_gf_rshift1(BN_128 r, const BN_128 a){
	r[1]=(a[0]<<63)|(a[1]>>1);
	r[0]=a[0]>>1;
}

void bn_128_gf_add(BN_128 r, const BN_128 a, const BN_128 b){
	r[0]=a[0]^b[0];
	r[1]=a[1]^b[1];
}

void bn_128_gf_mul(BN_128 r, const BN_128 a, const BN_128 b){
	BN_128 v,R={0,0};
	v[0]=a[0];v[1]=a[1];
	for(int i=63;i>=0;--i){
		if((b[0]>>i)&0x01ULL){
			bn_128_gf_add(R,R,v);
		}
		if(v[1]&0x01ULL){
			bn_128_gf_rshift1(v,v);
			v[0]^=0xE100000000000000ULL;
		}else{
			bn_128_gf_rshift1(v,v);
		}
	}
	for(int i=63;i>=0;--i){
		if((b[1]>>i)&0x01ULL){
			bn_128_gf_add(R,R,v);
		}
		if(v[1]&0x01ULL){
			bn_128_gf_rshift1(v,v);
			v[0]^=0xE100000000000000ULL;
		}else{
			bn_128_gf_rshift1(v,v);
		}
	}
	r[0]=R[0];
	r[1]=R[1];
}

void bn_128_to_bin(const BN_128 r, uint8_t dst[16]){
	((uint64_t*)dst)[0]=h2be_64(r[0]);
	((uint64_t*)dst)[1]=h2be_64(r[1]);
}

void bn_128_from_bin(BN_128 r, const uint8_t src[16]){
	r[0]=be2h_64(((uint64_t*)src)[0]);
	r[1]=be2h_64(((uint64_t*)src)[1]);

}

void ghash_init(GHASH_CTX* ctx, const uint8_t key[16]){
	bn_128_from_bin(ctx->H,key);
	ctx->Y[0]=0;
	ctx->Y[1]=0;
}

void ghash_update(GHASH_CTX* ctx, const uint8_t message[16]){
	BN_128 X;
	bn_128_from_bin(X,message);
	bn_128_gf_add(ctx->Y,X,ctx->Y);
	bn_128_gf_mul(ctx->Y,ctx->Y,ctx->H);
}

void ghash_final(GHASH_CTX* ctx, uint8_t hash[16]){
	bn_128_to_bin(ctx->Y,hash);
}
