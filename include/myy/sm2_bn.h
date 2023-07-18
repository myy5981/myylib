#ifndef MYY_SM2_BN_H
#define MYY_SM2_BN_H

#include <myy/env.h>
#include <myy/bignum.h>

__CPP_BEGIN

extern	const	BN_256	SM2_P;
#define	BN_256_P	BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF)

/* 定义在素数域GF(P)上的256位整数 */
typedef	BN_256		BN_256_GFp;
typedef	BN_256_ptr	BN_256_GFp_ptr;

extern	void	bn_256_GFp_add	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_sub	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_mul	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_neg	(BN_256_GFp r, const BN_256_GFp a);

#define	bn_256_GFp_dbl(r,a)	bn_256_GFp_add(r,a,a)
// 不支持内存重叠，以后会改
#define	bn_256_GFp_tri(r,a)	bn_256_GFp_add(r,a,a);bn_256_GFp_add(r,r,a)
#define	bn_256_GFp_sqr(r,a)	bn_256_GFp_mul(r,a,a)

__CPP_END

#endif