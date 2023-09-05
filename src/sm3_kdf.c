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

/**
 * 2023-09-05 记一次发生在半年前的逆天错误
 * 第58行处本来写的是：if(sm3_update(&sm3_ctx,(uint8_t*)&ct,4)!=0){
 * 正确的写法应该是将ct换成ct_be，但是当初没发现
 * 现在想来当初编译器已经提示我了，说我ct_be变量初始化了但未使用
 * （我开启了-Wall -Werror选项，即打印所有警告&所有警告均视为错误）
 * 又但是我还是没发现这个错误，于是只当gcc抽风了，加上了一行ct_be=ct_be消除了这个警告
 * 然后有段时间没碰代码，就忘了这事
 * 直到今天写SM2密钥交换时用到了这个函数，发现kdf函数输入正确，输出错误
 * （然而在这这前更是花了半个h排查是不是输入数据错误）
 * 然后才发现这个错误，（顺带发现了计数变量的初始值错误）
 * 这件事情告诉我们：
 * 	1. 编译器一般不会抽风，多半是自己抽风
 * 	2. 开启-Wall -Werror很重要
 * 	3. 认真对待每一个警告更重要，不要使用某N上的过于逆天的消除警告的办法
 * 2023-09-05 立碑，望后人哀之鉴之
*/
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
