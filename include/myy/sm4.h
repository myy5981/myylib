#ifndef MYY_SM4_H
#define MYY_SM4_H

#include <myy/env.h>
#include <myy/ghash.h>

__CPP_BEGIN

/**
 * SM4_KEY is an array with 32 uint32_t,
 * never use SM4_KEY*, use SM4_KEY instead,
 * you are supposed to know this
 */
typedef uint32_t	SM4_KEY[32];

extern	void	sm4_key_init		(SM4_KEY key, const uint8_t origin[16]);
extern	void	sm4_encrypt_block	(SM4_KEY key, const uint8_t message[16], uint8_t cipher[16]);
extern	void	sm4_decrypt_block	(SM4_KEY key, const uint8_t cipher[16], uint8_t message[16]);

#define	sm4_ecb_init	sm4_key_init
#define	sm4_ecb_encrypt	sm4_encrypt_block
#define	sm4_ecb_decrypt	sm4_decrypt_block

extern	void	sm4_encrypt_block_with_xor	(SM4_KEY key, const uint8_t message[16], const uint8_t perfix[16], uint8_t cipher[16]);
extern	void	sm4_decrypt_block_with_xor	(SM4_KEY key, const uint8_t cipher[16], const uint8_t perfix[16], uint8_t message[16]);

#define	sm4_cbc_init	sm4_key_init
#define	sm4_cbc_encrypt	sm4_encrypt_block_with_xor
#define	sm4_cbc_decrypt	sm4_decrypt_block_with_xor

typedef struct _SM4_GCM_CTX{
	SM4_KEY key;			/* SM4加密密钥 */
	GHASH_CTX ctx4ghash;	/* ghash上下文 */
	uint8_t icb[16];		/* 计数器（含IV） */
	uint8_t block[16];		/* 备份不完整的密文块 */
	size_t aad_size;		/* 附加数据长度，单位bit */
	uint32_t ct;			/* 计数变量 */
	int block_l;			/* block有效数据长度 */
}SM4_GCM_CTX;

/**
 * key：SM4初始密钥
 * IV：初始向量，固定为96bit（12字节）
 * aad：附加鉴别数据，其长度（字节）由aad_len指定。其可以为NULL，若为NULL则aad_len参数被忽略。
 * 错误（当且仅当aad_len过长，即add_len*8>2^64-1）时返回-1，成功返回0
*/
extern	int		sm4_gcm_init		(SM4_GCM_CTX* ctx, const uint8_t key[16], const uint8_t IV[12], const uint8_t* aad, const size_t aad_len);

/**
 * 重置ctx。不改变加密密钥，仅重置IV与aad
*/
extern	int		sm4_gcm_reset		(SM4_GCM_CTX* ctx, const uint8_t IV[12], const uint8_t* aad, const size_t aad_len);

/**
 * 该函数加密len字节m并写入c，写入数据和len等长，并返回0
 * 错误（当且仅当总的加密数据过长）时返回-1
*/
extern	int		sm4_gcm_enc_update	(SM4_GCM_CTX* ctx, uint8_t* c, const uint8_t* m, size_t len);

/**
 * 向ghash中写入t字节的本次加密的校验值，t不大于16
 * 当且仅当t不属于区间[1,16]时错误并返回-1
 * 否则返回0
*/
extern	int		sm4_gcm_enc_final	(SM4_GCM_CTX* ctx, uint8_t* gmac, int t);

/**
 * 该函数首先校验本次加密的消息认证码（由包含t字节的gmac指定），
 * 若不通过则返回1，且不会向m中写入数据，
 * 否则返回0，并向m中写入len字节由c解密的数据。
 * 在调用该函数前需要使用与加密的相同的参数调用sm4_gcm_init来初始化上下文
*/
extern	int		sm4_gcm_decrypt		(SM4_GCM_CTX* ctx, uint8_t* m, const uint8_t* c, size_t len, const uint8_t* gmac, int t);

__CPP_END

#endif
