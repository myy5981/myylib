#ifndef MYY_SM3_H
#define MYY_SM3_H

#include <myy/env.h>

__CPP_BEGIN

#define SM3_MAX_LEN		2305843009213693952ULL
#define SM3_BLOCK_LEN	64U
#define SM3_DIGEST_LEN	32U

struct _SM3_CTX {
	uint32_t W[68];
	uint32_t V[8];
	size_t len;
	int num;
};

typedef	struct _SM3_CTX	SM3_CTX;

/*如果使用宏SM3_CTX_STATIC_INIT进行静态初始化，则无需调用sm3_init函数进行初始化*/
#define	SM3_CTX_STATIC_INIT	{.len=0,.num=0,.V={0x7380166FU,0x4914B2B9U,0x172442D7U,0xDA8A0600U,0xA96F30BCU,0x163138AAU,0xE38DEE4DU,0xB0FB0E4EU},.W={0}}

extern	void	sm3_init		(SM3_CTX* ctx);
/*SM3对数据量有限制（小于2^64bit，也就是2^61字节），如果数据量过大则返回-1，否则只会返回0*/
extern	int		sm3_update		(SM3_CTX* ctx, const uint8_t* data, size_t len);
extern	void	sm3_final		(SM3_CTX* ctx, void* dest);
extern	int		sm3_hash		(void* message, size_t len, void* dest);
extern	void	sm3_ctx_clone	(SM3_CTX* src, SM3_CTX* dest);

__CPP_END

#endif
