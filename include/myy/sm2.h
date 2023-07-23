#ifndef MYY_SM2_H
#define MYY_SM2_H

#include <myy/env.h>
#include <myy/sm2_bn.h>

__CPP_BEGIN

struct _SM2_JPOINT {
	BN_256 x;
	BN_256 y;
	BN_256 z;
};

struct _SM2_POINT {
	BN_256 x;
	BN_256 y;
};

typedef struct _SM2_JPOINT	SM2_JPOINT;
typedef struct _SM2_POINT	SM2_POINT;

#define	sm2_jpoint_is_zero(p)	bn_256_is_zero((p)->z)
#define	sm2_jpoint_cpy(r,a)		bn_256_cpy((r)->x,(a)->x);bn_256_cpy((r)->y,(a)->y);bn_256_cpy((r)->z,(a)->z)

extern	void	sm2_jpoint_to_point		(SM2_POINT* r, const SM2_JPOINT* a);
extern	void	sm2_point_to_jpoint		(SM2_JPOINT* r, const SM2_POINT* a);
extern	void	sm2_jpoint_dbl			(SM2_JPOINT* r, const SM2_JPOINT* a);
extern	void	sm2_jpoint_add_point	(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_POINT* b);
extern	void	sm2_point_mul			(SM2_JPOINT* r, const SM2_POINT* a, const BN_256 k);

__CPP_END

#endif
