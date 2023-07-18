#ifndef MYY_SM3_KDF_H
#define MYY_SM3_KDF_H

#include <myy/sm3.h>

__CPP_BEGIN

struct _SM3_KDF_CTX {
	SM3_CTX sm3_ctx4kdf;
	uint32_t ct;
};

typedef	struct _SM3_KDF_CTX	SM3_KDF_CTX;

extern	void	sm3_kdf_init		(SM3_KDF_CTX* ctx);
extern	int		sm3_kdf_init_update	(SM3_KDF_CTX* ctx, uint8_t* data, size_t len);

/**
 * 当计数器超过最大值（含最大值，即：ct>=0X7FFFFFFF）时，或
 * 超过sm3支持的最大数据量时，返回-1
 * 否则向dest中写入256比特的密钥流，并返回0
 * 该函数是有状态的，每成功调用一次会将计数器+1
 */
extern	int		sm3_kdf_next		(SM3_KDF_CTX* ctx, void* dest);
/**
 * 向dest中输出长度为klen字节的密钥流
 * 根据国密标准：klen*8 < (2^32-1)*256
 */
extern	int		sm3_kdf_final		(SM3_KDF_CTX* ctx, void* dest, size_t klen);
extern	int		sm3_kdf				(void* message, size_t len, void* dest, size_t klen);

__CPP_END

#endif
