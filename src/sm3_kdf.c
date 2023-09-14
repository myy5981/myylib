#include <myy/sm3_kdf.h>
#include <myy/endian.h>

#include <string.h>

void sm3_kdf_init(SM3_KDF_CTX* ctx){
	sm3_init(&ctx->sm3_ctx4kdf);
	ctx->ct=1;
}

int sm3_kdf_init_update(SM3_KDF_CTX* ctx,const uint8_t* data,size_t len){
	return sm3_update(&ctx->sm3_ctx4kdf,data,len);
}

int sm3_kdf_next(SM3_KDF_CTX* ctx,void* dest){
	if(ctx->ct>=0x7FFFFFFFU){
		return -1;
	}
	SM3_CTX sm3_ctx=ctx->sm3_ctx4kdf;
	uint32_t ct=h2be_32(ctx->ct);
	if(sm3_update(&sm3_ctx,(uint8_t*)&ct,4)==0){
		sm3_final(&sm3_ctx,dest);
		ctx->ct++;
		return 0;
	}else{
		return -1;
	}
}

int sm3_kdf_final(SM3_KDF_CTX* ctx, void* dest, size_t klen){
	if(klen>=0x0000001FFFFFFFE0ULL){/* klen*8 >= (2^32-1)*256 */
		return -1;
	}
	uint32_t ct=klen>>5;
	uint32_t ct_be=0;
	uint32_t i=1;
	SM3_CTX sm3_ctx;
	for(;i<=ct;++i) {
		sm3_ctx=ctx->sm3_ctx4kdf;
		ct_be=h2be_32(i);
		if(sm3_update(&sm3_ctx,(uint8_t*)&ct_be,4)!=0){
			return -1;
		}
		sm3_final(&sm3_ctx,dest);
	}
	if((klen&0x1F)!=0){
		uint8_t buf[SM3_DIGEST_LEN];
		sm3_ctx=ctx->sm3_ctx4kdf;
		ct_be=h2be_32(i);
		if(sm3_update(&sm3_ctx,(uint8_t*)&ct_be,4)!=0){
			return -1;
		}
		sm3_final(&sm3_ctx,buf);
		memcpy(dest,buf,klen&0x1F);
	}
	return 0;
}

int sm3_kdf(void* message, size_t len, void* dest, size_t klen){
	if(klen>=0x0000001FFFFFFFE0ULL){/* klen*8 >= (2^32-1)*256 */
		return -1;
	}
	SM3_CTX ctx=SM3_CTX_STATIC_INIT;
	if(sm3_update(&ctx,(uint8_t*)message,len)!=0){
		return -1;
	}
	uint32_t ct=klen>>5;
	uint32_t ct_be=0;
	uint32_t i=1;
	SM3_CTX sm3_ctx;
	for(;i<=ct;++i) {
		sm3_ctx=ctx;
		ct_be=h2be_32(i);
		if(sm3_update(&sm3_ctx,(uint8_t*)&ct_be,4)!=0){
			return -1;
		}
		sm3_final(&sm3_ctx,dest);
	}
	if((klen&0x1F)!=0){
		uint8_t buf[SM3_DIGEST_LEN];
		sm3_ctx=ctx;
		ct_be=h2be_32(i);
		if(sm3_update(&sm3_ctx,(uint8_t*)&ct_be,4)!=0){
			return -1;
		}
		sm3_final(&sm3_ctx,buf);
		memcpy(dest,buf,klen&0x1F);
	}
	return 0;
}
