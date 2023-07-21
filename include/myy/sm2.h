#ifndef MYY_SM2_H
#define MYY_SM2_H

#include <myy/env.h>
#include <myy/sm2_bn.h>

__CPP_BEGIN

struct _JACOBIAN_POINT {
	BN_256 x;
	BN_256 y;
	BN_256 z;
};
typedef	struct _JACOBIAN_POINT	JACOBIAN_POINT;

struct _SM2_POINT {
	BN_256 x;
	BN_256 y;
};
typedef struct _SM2_POINT SM2_POINT;

#define	sm2_point_is_zero(p)	bn_256_is_zero((p)->z)
#define	sm2_point_cpy(r,a)		bn_256_cpy((r)->x,(a)->x);bn_256_cpy((r)->y,(a)->y);bn_256_cpy((r)->z,(a)->z)

extern	void	sm2_jacobian_to_point	(SM2_POINT* r, const JACOBIAN_POINT* a);
extern	void	sm2_point_dbl			(JACOBIAN_POINT* r, const JACOBIAN_POINT* a);
extern	void	sm2_point_add			(JACOBIAN_POINT* r, const JACOBIAN_POINT* a, const JACOBIAN_POINT* b);
extern	void	sm2_point_mul			(JACOBIAN_POINT* r, const JACOBIAN_POINT* a, const BN_256 k);

__CPP_END

#endif
