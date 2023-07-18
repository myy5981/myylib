#ifndef MYY_SM4_H
#define MYY_SM4_H

#include <myy/env.h>

__CPP_BEGIN

/**
 * SM4_KEY is an array with 32 uint32_t,
 * never use SM4_KEY*, use SM4_KEY instead,
 * you are supposed to know this
 */
typedef uint32_t	SM4_KEY[32];

extern	void	sm4_key_init		(SM4_KEY key, uint8_t origin[16]);
extern	void	sm4_encrypt_block	(SM4_KEY key, uint8_t message[16], uint8_t cipher[16]);
extern	void	sm4_decrypt_block	(SM4_KEY key, uint8_t cipher[16], uint8_t message[16]);

#define	sm4_ecb_init	sm4_key_init
#define	sm4_ecb_encrypt	sm4_encrypt_block
#define	sm4_ecb_decrypt	sm4_decrypt_block

extern	void	sm4_encrypt_block_with_xor	(SM4_KEY key, uint8_t message[16], uint8_t perfix[16], uint8_t cipher[16]);
extern	void	sm4_decrypt_block_with_xor	(SM4_KEY key, uint8_t cipher[16], uint8_t perfix[16], uint8_t message[16]);

#define	sm4_cbc_init	sm4_key_init
#define	sm4_cbc_encrypt	sm4_encrypt_block_with_xor
#define	sm4_cbc_decrypt	sm4_decrypt_block_with_xor

__CPP_END

#endif
