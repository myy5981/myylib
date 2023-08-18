#ifndef MYY_SM2_BN_H
#define MYY_SM2_BN_H

#include <myy/env.h>
#include <myy/bignum.h>

__CPP_BEGIN

extern const BN_256 SM2_P;

/* 定义在素数域GF(P)上的256位整数 */
typedef	BN_256		BN_256_GFp;
typedef	BN_256_ptr	BN_256_GFp_ptr;

extern	void	bn_256_GFp_add	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_sub	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_neg	(BN_256_GFp r, const BN_256_GFp a);
extern	void	bn_256_GFp_mul	(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b);
extern	void	bn_256_GFp_exp	(BN_256_GFp r, const BN_256_GFp a, const BN_256 e);
extern	void	bn_256_GFp_inv	(BN_256_GFp r, const BN_256_GFp a);
extern	int		bn_256_GFp_sqrt	(BN_256_GFp r, const BN_256_GFp a);
#define	bn_256_GFp_dbl(r,a)		bn_256_GFp_add(r,a,a)
#define	bn_256_GFp_sqr(r,a)		bn_256_GFp_mul(r,a,a)
#define	bn_256_GFp_cpy(r,a)		bn_256_cpy(r,a)
#define	bn_256_GFp_cmp(a,b)		bn_256_cmp(a,b)
#define	bn_256_GFp_is_zero(a)	bn_256_is_zero(a)
#define	bn_256_GFp_is_one(a)	bn_256_is_one(a)
#define	bn_256_GFp_set_zero(a)	bn_256_set_zero(a)
#define	bn_256_GFp_set_one(a)	bn_256_set_one(a)

/* 蒙哥马利模乘，在进行单次乘法取模时，效率不如巴雷特约减，优势在连乘时才会发挥出来 */

/* 定义在素数域GF(P)上的256位整数的蒙哥马利形式 */
typedef BN_256		BN_256_GFp_Mont;
typedef	BN_256_ptr	BN_256_GFp_Mont_ptr;

extern	void	bn_256_mont_to_GFp		(BN_256_GFp r, const BN_256_GFp_Mont X);
extern	void	bn_256_GFp_to_mont		(BN_256_GFp_Mont r, const BN_256_GFp X);
extern	void	bn_256_GFp_mont_redc	(BN_256_GFp_Mont r, const BN_512 X);
extern	void	bn_256_GFp_mul_mont		(BN_256_GFp_Mont r, const BN_256_GFp_Mont a, const BN_256_GFp_Mont b);
extern	void	bn_256_GFp_inv_mont		(BN_256_GFp_Mont r, const BN_256_GFp_Mont a);
/**
 * 计算a的一个平方根置于r中，若a不存在平方根，则函数返回非零值
*/
extern	int		bn_256_GFp_sqrt_mont	(BN_256_GFp_Mont r, const BN_256_GFp_Mont a);
extern	int		bn_256_GFp_is_one_mont	(const BN_256_GFp_Mont a);
extern	void	bn_256_GFp_set_one_mont	(BN_256_GFp_Mont a);
#define bn_256_GFp_add_mont(r,a,b)		bn_256_GFp_add(r,a,b)
#define bn_256_GFp_dbl_mont(r,a)		bn_256_GFp_add(r,a,a)
#define bn_256_GFp_sub_mont(r,a,b)		bn_256_GFp_sub(r,a,b)
#define bn_256_GFp_neg_mont(r,a)		bn_256_GFp_neg(r,a)
#define	bn_256_GFp_sqr_mont(r,a)		bn_256_GFp_mul_mont(r,a,a)
#define	bn_256_GFp_cpy_mont(r,a)		bn_256_cpy(r,a)
#define bn_256_GFp_eql_mont(a,b)		(bn_256_cmp(a,b)==0)
#define bn_256_GFp_is_zero_mont(n)		bn_256_is_zero(n)
#define bn_256_GFp_set_zero_mont(n)		bn_256_set_zero(n)
__CPP_END

#endif