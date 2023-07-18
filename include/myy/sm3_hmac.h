#ifndef MYY_SM3_HMAC_H
#define MYY_SM3_HMAC_H

#include <myy/sm3.h>

__CPP_BEGIN

#define SM3_HMAC_KEY_LEN SM3_BLOCK_LEN

struct _SM3_HMAC_CTX {
	SM3_CTX sm3_ctx4hmac;
	uint8_t key[SM3_HMAC_KEY_LEN];
};

typedef struct _SM3_HMAC_CTX SM3_HMAC_CTX;

extern	int		sm3_hmac_init	(SM3_HMAC_CTX* ctx, uint8_t* key, size_t key_len);
extern	int		sm3_hmac_update	(SM3_HMAC_CTX* ctx, uint8_t* data, size_t len);
extern	void	sm3_hmac_final	(SM3_HMAC_CTX* ctx, void* dest);
extern	int		sm3_hmac		(void* message, size_t len, uint8_t* key, size_t key_len, void* dest);

/**
 * 从已经初始化的SM3_HMAC_CTX分离出单独的SM3_CTX，即将ctx->sm3_ctx4hmac拷贝到detach中
 * 当需要并发地对多条数据使用相同的密钥计算hmac时可以使用该方法以免多余的内存空间与密钥初始化损耗
 * 但在最终计算时由于需要密钥参与，仍然需要在final函数中引入完整的SM3_HMAC_CTX（只读的）
 * 无需对detach使用sm3_init
 * 除了调用sm3_hmac_detach函数外，
 *	  您也可以：SM3_CTX detach = ctx->sm3_ctx4hmac，或
 *	  使用sm3_hmac_DETACH宏：SM3_CTX detach = sm3_hmac_DETACH(ctx)
 */
#define	sm3_hmac_DETACH(ctx)	(ctx->sm3_ctx4hmac)
#define	sm3_hmac_detach_update	sm3_update
extern	void	sm3_hmac_detach			(SM3_HMAC_CTX* ctx, SM3_CTX* detach);
extern	void	sm3_hmac_detach_final	(SM3_HMAC_CTX* ctx, SM3_CTX* detach, void* dest);

__CPP_END

#endif
