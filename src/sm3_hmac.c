#include <myy/sm3_hmac.h>

#include <string.h>

#define IPAD	0x36U
#define IPAD_32	0x36363636U
#define OPAD	0x5CU
#define OPAD_32	0x5C5C5C5CU

int sm3_hmac_init(SM3_HMAC_CTX* ctx,uint8_t* key,size_t key_len){
	if(key_len>=SM3_MAX_LEN){
		return -1;
	}
	sm3_init(&ctx->sm3_ctx4hmac);
	if(key_len<=SM3_HMAC_KEY_LEN){
		memcpy(ctx->key,key,key_len);
	}else{
		sm3_update(&ctx->sm3_ctx4hmac,key,key_len);
		sm3_final(&ctx->sm3_ctx4hmac,ctx->key);
		key_len=SM3_DIGEST_LEN;
	}
	int i;
	for(i=0;i<key_len;++i){
		ctx->key[i]^=IPAD;
	}
	memset(ctx->key+key_len,0^IPAD,SM3_HMAC_KEY_LEN-key_len);
	sm3_init(&ctx->sm3_ctx4hmac);
	sm3_update(&ctx->sm3_ctx4hmac,ctx->key,SM3_HMAC_KEY_LEN);
	for(i=0;i<SM3_HMAC_KEY_LEN/4;++i){
		((uint32_t*)(ctx->key))[i]^=(IPAD_32^OPAD_32);
	}
	return 0;
}

int sm3_hmac_update(SM3_HMAC_CTX* ctx,uint8_t* data,size_t len){
	return sm3_update(&ctx->sm3_ctx4hmac,data,len);
}

void sm3_hmac_final(SM3_HMAC_CTX* ctx,void* dest){
	uint8_t hash[SM3_DIGEST_LEN];
	sm3_final(&ctx->sm3_ctx4hmac,hash);
	sm3_init(&ctx->sm3_ctx4hmac);
	sm3_update(&ctx->sm3_ctx4hmac,ctx->key,SM3_HMAC_KEY_LEN);
	sm3_update(&ctx->sm3_ctx4hmac,hash,SM3_DIGEST_LEN);
	sm3_final(&ctx->sm3_ctx4hmac,dest);
}

int sm3_hmac(void* message,size_t len,uint8_t* key,size_t key_len,void* dest){
	if(len>=SM3_MAX_LEN||key_len>=SM3_MAX_LEN){
		return -1;
	}
	SM3_HMAC_CTX ctx;
	sm3_hmac_init(&ctx,key,key_len);
	sm3_hmac_update(&ctx,message,len);
	sm3_hmac_final(&ctx,dest);
	return 0;
}

void sm3_hmac_detach(SM3_HMAC_CTX* ctx,SM3_CTX* detach){
	sm3_ctx_clone(&ctx->sm3_ctx4hmac,detach);
}

void sm3_hmac_detach_final(SM3_HMAC_CTX* ctx,SM3_CTX* detach,void* dest){
	uint8_t hash[SM3_DIGEST_LEN];
	sm3_final(detach,hash);
	sm3_init(detach);
	sm3_update(detach,ctx->key,SM3_HMAC_KEY_LEN);
	sm3_update(detach,hash,SM3_DIGEST_LEN);
	sm3_final(detach,dest);
}
