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

typedef struct _JACOBIAN_POINT JACOBIAN_POINT;

#define sm2_point_is_zero(p) bn_256_is_zero((p)->z)
#define sm2_point_cpy(r,a) bn_256_cpy((r)->x,(a)->x);bn_256_cpy((r)->y,(a)->y);bn_256_cpy((r)->z,(a)->z)

void sm2_point_dbl  (JACOBIAN_POINT* r, JACOBIAN_POINT* a);
void sm2_point_add  (JACOBIAN_POINT* r, JACOBIAN_POINT* a, JACOBIAN_POINT* b);

__CPP_END

#endif
