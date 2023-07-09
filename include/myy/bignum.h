#ifndef MYY_BIGNUM_H
#define MYY_BIGNUM_H

#include <myy/env.h>

__CPP_BEGIN

//大端存储，高32位存在低索引，低32位存在高索引
typedef uint32_t BN_256[8];
typedef uint32_t BN_512[16];

#define bn_256_is_zero(n) (((n)[0]==0)&&\
                                ((n)[1]==0)&&\
                                ((n)[2]==0)&&\
                                ((n)[3]==0)&&\
                                ((n)[4]==0)&&\
                                ((n)[5]==0)&&\
                                ((n)[6]==0)&&\
                                ((n)[7]==0))

#define bn_256_xor(r,a,b) (r)[0]=(a)[0]^(b)[0];\
                            (r)[1]=(a)[1]^(b)[1];\
                            (r)[2]=(a)[2]^(b)[2];\
                            (r)[3]=(a)[3]^(b)[3];\
                            (r)[4]=(a)[4]^(b)[4];\
                            (r)[5]=(a)[5]^(b)[5];\
                            (r)[6]=(a)[6]^(b)[6];\
                            (r)[7]=(a)[7]^(b)[7]

#define bn_256_and(r,a,b) (r)[0]=(a)[0]&(b)[0];\
                            (r)[1]=(a)[1]&(b)[1];\
                            (r)[2]=(a)[2]&(b)[2];\
                            (r)[3]=(a)[3]&(b)[3];\
                            (r)[4]=(a)[4]&(b)[4];\
                            (r)[5]=(a)[5]&(b)[5];\
                            (r)[6]=(a)[6]&(b)[6];\
                            (r)[7]=(a)[7]&(b)[7]

#define bn_256_left(n,x) (n)[0]=((n)[0]<<x)|((n)[1]>>(32-x));\
                            (n)[1]=((n)[1]<<x)|((n)[2]>>(32-x));\
                            (n)[2]=((n)[2]<<x)|((n)[3]>>(32-x));\
                            (n)[3]=((n)[3]<<x)|((n)[4]>>(32-x));\
                            (n)[4]=((n)[4]<<x)|((n)[5]>>(32-x));\
                            (n)[5]=((n)[5]<<x)|((n)[6]>>(32-x));\
                            (n)[6]=((n)[6]<<x)|((n)[7]>>(32-x));\
                            (n)[7]=((n)[7]<<x)

extern void __bn_256_add    (BN_256 r, const BN_256 a, const BN_256 b);
/**
 * 如果发生进位则返回1，否则返回0
*/
extern int  bn_256_add      (BN_256 r, const BN_256 a, const BN_256 b);
extern void bn_256_sub      (BN_256 r, const BN_256 a, const BN_256 b);
extern void bn_256_mul      (BN_512 r, BN_256 a, BN_256 b);
extern int  bn_256_cmp      (const BN_256 a, const BN_256 b);
extern void bn_256_zero     (BN_256 bn);

extern int  bin_2_bn_256    (BN_256 bn, const char* bin);
extern void bn_2_bin_256    (BN_256 bn, char* bin);
extern int  hex_2_bn_256    (BN_256 bn, const char* hex);

#define BN_ZERO_256 {0,0,0,0,0,0,0,0}
#define BN_ONE_256  {0,0,0,0,0,0,0,1}

__CPP_END

#endif
