#ifndef MYY_BIGNUM_H
#define MYY_BIGNUM_H

#include <myy/env.h>

__CPP_BEGIN

//大端存储，高32位存在低索引，低32位存在高索引
#define BN_256_LEN 8
#define BN_512_LEN 16
#define UNIT_LEN 32

typedef uint32_t BN_256[BN_256_LEN];
typedef uint32_t BN_512[BN_512_LEN];

#define BN_256_INIT(a,b,c,d,e,f,g,h) {0x##a##U,0x##b##U,0x##c##U,0x##d##U,0x##e##U,0x##f##U,0x##g##U,0x##h##U}
#define BN_256_ZERO {0,0,0,0,0,0,0,0}
#define BN_256_ONE  {0,0,0,0,0,0,0,1}

#define bn_256_is_zero(n) (((n)[0]==0)&&\
                                ((n)[1]==0)&&\
                                ((n)[2]==0)&&\
                                ((n)[3]==0)&&\
                                ((n)[4]==0)&&\
                                ((n)[5]==0)&&\
                                ((n)[6]==0)&&\
                                ((n)[7]==0))

#define bn_256_is_odd(n) (n[7]&0x01)

#define bn_256_xor(r,a,b) (r)[0]=(a)[0]^(b)[0];\
                            (r)[1]=(a)[1]^(b)[1];\
                            (r)[2]=(a)[2]^(b)[2];\
                            (r)[3]=(a)[3]^(b)[3];\
                            (r)[4]=(a)[4]^(b)[4];\
                            (r)[5]=(a)[5]^(b)[5];\
                            (r)[6]=(a)[6]^(b)[6];\
                            (r)[7]=(a)[7]^(b)[7]

#define bn_256_cpy(r,a) (r)[0]=(a)[0];\
                            (r)[1]=(a)[1];\
                            (r)[2]=(a)[2];\
                            (r)[3]=(a)[3];\
                            (r)[4]=(a)[4];\
                            (r)[5]=(a)[5];\
                            (r)[6]=(a)[6];\
                            (r)[7]=(a)[7]

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

#define bn_256_right(n,x) (n)[0]=((n)[0]>>x)|((n)[1]<<(32-x));\
                            (n)[1]=((n)[1]>>x)|((n)[2]<<(32-x));\
                            (n)[2]=((n)[2]>>x)|((n)[3]<<(32-x));\
                            (n)[3]=((n)[3]>>x)|((n)[4]<<(32-x));\
                            (n)[4]=((n)[4]>>x)|((n)[5]<<(32-x));\
                            (n)[5]=((n)[5]>>x)|((n)[6]<<(32-x));\
                            (n)[6]=((n)[6]>>x)|((n)[7]<<(32-x));\
                            (n)[7]=((n)[7]>>x)

/* 如果发生进位则返回1，否则返回0 */
extern int  bn_256_add      (BN_256 r, const BN_256 a, const BN_256 b);
extern void __bn_256_add    (BN_256 r, const BN_256 a, const BN_256 b);
extern void bn_256_sub      (BN_256 r, const BN_256 a, const BN_256 b);
extern void bn_256_mul      (BN_512 r, const BN_256 a, const BN_256 b);
extern int  bn_256_cmp      (const BN_256 a, const BN_256 b);

extern void bn_256_set_zero (BN_256 bn);
extern void bn_256_set_word (BN_256 bn, uint32_t w);

extern void bn_256_to_bytes (BN_256 bn, uint8_t* dest);

__CPP_END

#endif
