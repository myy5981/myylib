#ifndef MYY_BN128_H
#define MYY_BN128_H

#include <myy/env.h>

__CPP_BEGIN

typedef	uint64_t	BN_128[2];
typedef	uint64_t*	BN_128_ptr;

extern	void	bn_128_gf_rshift1	(BN_128 r, const BN_128 a);
extern	void	bn_128_gf_add		(BN_128 r, const BN_128 a, const BN_128 b);
extern	void	bn_128_gf_mul		(BN_128 r, const BN_128 a, const BN_128 b);
extern	void	bn_128_to_bin		(const BN_128 r, uint8_t dst[16]);
extern	void	bn_128_from_bin		(BN_128 r, const uint8_t src[16]);

typedef struct _GHASH_CTX{
	BN_128 H;
	BN_128 Y;
}GHASH_CTX;

extern	void	ghash_init	(GHASH_CTX* ctx, const uint8_t key[16]);
extern	void	ghash_update(GHASH_CTX* ctx, const uint8_t message[16]);
extern	void	ghash_final	(GHASH_CTX* ctx, uint8_t hash[16]);

__CPP_END

#endif
