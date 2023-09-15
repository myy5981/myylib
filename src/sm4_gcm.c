#include <myy/sm4.h>
#include <myy/endian.h>

#include <string.h>

int sm4_gcm_init(SM4_GCM_CTX* ctx,
					const uint8_t key[16], const uint8_t IV[12],
					const uint8_t* aad, const size_t aad_len){
	if(aad_len>=UINT64_MAX>>3){
		return -1;
	}
	sm4_key_init(ctx->key,key);
	ctx->ctx4ghash.Y[0]=0;
	ctx->ctx4ghash.Y[1]=0;
	sm4_encrypt_block(ctx->key,(uint8_t*)ctx->ctx4ghash.Y,(uint8_t*)ctx->ctx4ghash.H);
	bn_128_from_bin(ctx->ctx4ghash.H,(uint8_t*)ctx->ctx4ghash.H);
	ctx->aad_size=0;
	if(aad!=NULL){
		for (size_t i = 0; i < aad_len/16; i++) {
			ghash_update(&(ctx->ctx4ghash),aad);
			aad+=16;
		}
		int r=aad_len%16;
		if(r!=0){
			uint8_t buf[16];
			memcpy(buf,aad,r);
			memset(buf+r,0,16-r);
		}
		ctx->aad_size=aad_len*8;
	}
	memcpy(ctx->cb,IV,12);
	ctx->cb[15]=1;
	ctx->cb[14]=0;
	ctx->cb[13]=0;
	ctx->cb[12]=0;
	ctx->block_l=0;
	return 0;
}

static int sm4_gcm_enc_update_block(SM4_GCM_CTX* ctx,const uint8_t m[16],uint8_t c[16]){
	uint32_t ct=*(uint32_t*)(ctx->cb+12);
	ct=be2h_32(ct);
	if(ct>=UINT32_MAX){
		return 1;
	}
	
	uint8_t s[16];
	sm4_encrypt_block(ctx->key,ctx->cb,s);
	((uint64_t*)c)[0]=((uint64_t*)m)[0]^((uint64_t*)s)[0];
	((uint64_t*)c)[1]=((uint64_t*)m)[1]^((uint64_t*)s)[1];
	ghash_update(&ctx->ctx4ghash,c);

	ct++;
	ct=h2be_32(ct);
	*(uint32_t*)(ctx->cb+12)=ct;
	return 0;
}

int sm4_gcm_enc_update(SM4_GCM_CTX* ctx, uint8_t* c, const uint8_t* m, size_t len){
	int r=0;
	if(ctx->block_l!=0){
		if(ctx->block_l+len<16){
			memcpy(ctx->block+ctx->block_l,m,len);
			ctx->block_l+=len;
			return r;
		}
		memcpy(ctx->block+ctx->block_l,m,16-ctx->block_l);
		if(sm4_gcm_enc_update_block(ctx,ctx->block,c)){
			return -1;
		}
		m+=(16-ctx->block_l);
		c+=16;
		len-=(16-ctx->block_l);
		ctx->block_l=0;
		r+=16;
	}
	for(size_t i=0; i<len/16; i++){
		if(sm4_gcm_enc_update_block(ctx,m,c)){
			return -1;
		}
		m+=16;
		c+=16;
		r+=16;
	}
	if(len%16!=0){
		ctx->block_l=len%16;
		memcpy(ctx->block,m,ctx->block_l);
	}
	return r;
}

int sm4_gcm_enc_final(SM4_GCM_CTX* ctx, uint8_t* c, uint8_t* gmac, int t){
	if(t<1||t>16){
		return -1;
	}
	int r=0;
	uint32_t ct=*(uint32_t*)(ctx->cb+12);
	ct=be2h_32(ct);
	uint8_t T[16];
	if(ctx->block_l!=0){
		if(ct>=UINT32_MAX){
			return 1;
		}
		r=ctx->block_l;
		sm4_encrypt_block(ctx->key,ctx->cb,T);
		((uint64_t*)T)[0]=((uint64_t*)T)[0]^((uint64_t*)(ctx->block))[0];
		((uint64_t*)T)[1]=((uint64_t*)T)[1]^((uint64_t*)(ctx->block))[1];
		memset(T+r,0,16-r);
		ghash_update(&ctx->ctx4ghash,T);
		memcpy(c,T,r);
	}else{
		ct--;
	}
	((uint64_t*)T)[0]=h2be_64(ctx->aad_size);
	size_t c_size=(size_t)ct*128ULL;
	((uint64_t*)T)[1]=h2be_64(c_size);
	ghash_update(&ctx->ctx4ghash,T);
	ghash_final(&ctx->ctx4ghash,T);

	ctx->cb[15]=1;
	ctx->cb[14]=0;
	ctx->cb[13]=0;
	ctx->cb[12]=0;
	uint8_t s[16];
	sm4_encrypt_block(ctx->key,ctx->cb,s);
	((uint64_t*)T)[0]=((uint64_t*)T)[0]^((uint64_t*)s)[0];
	((uint64_t*)T)[1]=((uint64_t*)T)[1]^((uint64_t*)s)[1];
	memcpy(gmac,T,t);
	return r;
}
