#ifndef MYY_BN128_H
#define MYY_BN128_H

#include <myy/env.h>

__CPP_BEGIN

typedef	uint64_t	BN_128[2];
typedef	uint64_t*	BN_128_ptr;

extern	void	bn_128_gf_rshift1	(BN_128 r, BN_128 a);
extern	void	bn_128_gf_add		(BN_128 r, BN_128 a, BN_128 b);
extern	void	bn_128_gf_mul		(BN_128 r, BN_128 a, BN_128 b);

__CPP_END

#endif
