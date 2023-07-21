#ifndef MYY_SM2_BN_H
#define MYY_SM2_BN_H

#include <myy/env.h>
#include <myy/bignum.h>

__CPP_BEGIN

extern const BN_256 SM2_P;

/* 定义在素数域GF(P)上的256位整数 */
typedef	BN_256		BN_256_GFp;
typedef	BN_256_ptr	BN_256_GFp_ptr;

/* 定义在素数域GF(P)上的256位整数的蒙哥马利形式 */
typedef BN_256		BN_256_Mont;
typedef	BN_256_ptr	BN_256_Mont_ptr;

extern	void	bn_256_GFp_add	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_sub	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_neg	(BN_256_GFp r, const BN_256_GFp a);
extern	void	bn_256_GFp_mul	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_exp	(BN_256_GFp r, const BN_256_GFp a, const BN_256 e);
extern	void	bn_256_GFp_inv	(BN_256_GFp r, const BN_256_GFp a);

#define	bn_256_GFp_dbl(r,a)	bn_256_GFp_add(r,a,a)
#define	bn_256_GFp_sqr(r,a)	bn_256_GFp_mul(r,a,a)

/* 蒙哥马利模乘，在进行单次乘法取模时，效率不如巴雷特约减，优势在连乘时才会发挥出来 */
extern	void	bn_256_mont_to_GFp	(BN_256_GFp r, const BN_256_Mont X);
extern	void	bn_256_GFp_to_mont	(BN_256_Mont r, const BN_256_GFp X);
extern	void	bn_256_GFp_mont_redc(BN_256_GFp r, const BN_512 X);
extern	void	bn_256_GFp_mul_mont	(BN_256_Mont r, const BN_256_Mont a, const BN_256_Mont b);

__CPP_END

#endif