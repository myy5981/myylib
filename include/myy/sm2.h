#ifndef MYY_SM2_H
#define MYY_SM2_H

#include <myy/env.h>
#include <myy/bignum.h>

__CPP_BEGIN

struct _JACOBIAN_POINT {
    BN_256 x;
    BN_256 y;
    BN_256 z;
};

typedef struct _JACOBIAN_POINT JACOBIAN_POINT;

__CPP_END

#endif
