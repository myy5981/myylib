#ifndef MYY_SM2_H
#define MYY_SM2_H

#include <myy/env.h>
#include <myy/sm2_bn.h>
#include <myy/sm3_kdf.h>

__CPP_BEGIN

/**
 * SM2椭圆曲线点运算
*/

typedef struct _SM2_JPOINT {
	BN_256 x;
	BN_256 y;
	BN_256 z;
} SM2_JPOINT;

typedef struct _SM2_POINT {
	BN_256 x;
	BN_256 y;
} SM2_POINT;

extern SM2_POINT SM2_G_MONT;

#define SM2_POINT_SERIALIZE_DEFAULT 0
#define SM2_POINT_SERIALIZE_COMPRESS 1
#define SM2_POINT_SERIALIZE_MIX 2

#define	sm2_jpoint_is_zero(p)	bn_256_GFp_is_zero((p)->z)
#define	sm2_point_is_zero(p)	(bn_256_GFp_is_zero((p)->x)&&bn_256_GFp_is_zero((p)->y))
#define	sm2_jpoint_cpy(r,a)		bn_256_cpy((r)->x,(a)->x);bn_256_cpy((r)->y,(a)->y);bn_256_cpy((r)->z,(a)->z)

extern	void	sm2_jpoint_to_point		(SM2_POINT* r, const SM2_JPOINT* a);
extern	void	sm2_point_to_jpoint		(SM2_JPOINT* r, const SM2_POINT* a);
extern	void	sm2_jpoint_dbl			(SM2_JPOINT* r, const SM2_JPOINT* a);
extern	void	sm2_jpoint_add_point	(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_POINT* b);
extern	void	sm2_point_mul			(SM2_JPOINT* r, const SM2_POINT* a, const BN_256 k);
extern	int		sm2_point_to_bin		(SM2_POINT* r, uint8_t* dst, int flag);
/* 暂时仅支持默认编码方式 */
extern	int		sm2_point_from_bin		(SM2_POINT* r, uint8_t* dst);

/* 基于蒙哥马利的点运算，效率比起上面的稍快一点，所有参与运算的点均需先转换为蒙哥马利域表示 */

#define	sm2_jpoint_is_zero_mont(p)			bn_256_GFp_is_zero_mont((p)->z)
#define	sm2_point_is_zero_mont(p)			(bn_256_GFp_is_zero_mont((p)->x)&&bn_256_GFp_is_zero_mont((p)->y))
extern	void	sm2_jpoint_to_point_mont	(SM2_POINT* r, const SM2_JPOINT* a);
extern	void	sm2_point_to_jpoint_mont	(SM2_JPOINT* r, const SM2_POINT* a);
extern	void	sm2_jpoint_dbl_mont			(SM2_JPOINT* r, const SM2_JPOINT* a);
extern	void	sm2_jpoint_add_mont			(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_JPOINT* b);
extern	void	sm2_jpoint_add_point_mont	(SM2_JPOINT* r, const SM2_JPOINT* a, const SM2_POINT* b);
extern	void	sm2_point_mul_mont			(SM2_JPOINT* r, const SM2_POINT* a, const BN_256 k);
extern	int		sm2_point_to_bin_mont		(SM2_POINT* r, uint8_t* dst, int flag);
/* 暂时仅支持默认编码方式 */
extern	int		sm2_point_from_bin_mont		(SM2_POINT* r, uint8_t* dst);

/* 将点转到蒙哥马利域的表示或从蒙哥马利域表示还原 */

extern	void	sm2_jpoint_to_mont			(SM2_JPOINT* r,SM2_JPOINT* a);
extern	void	sm2_jpoint_from_mont		(SM2_JPOINT* r,SM2_JPOINT* a);
extern	void	sm2_point_to_mont			(SM2_POINT* r,SM2_POINT* a);
extern	void	sm2_point_from_mont			(SM2_POINT* r,SM2_POINT* a);

/**
 * SM2 密钥生成与验证
*/

typedef SM2_POINT SM2_PUB_KEY;

typedef struct _SM2_PRI_KEY{
	BN_256 d;
	SM2_PUB_KEY Pub;
}SM2_PRI_KEY;

typedef struct _SM2_PRI_KEY_EXT{
	SM2_PRI_KEY key;
	BN_256_GFn d1_inv;
}SM2_PRI_KEY_EXT;

extern	void	sm2_key_generate		(SM2_PRI_KEY* prik);
extern	void	sm2_pub_key_generate	(SM2_PUB_KEY* pubk, SM2_PRI_KEY* prik);
extern	int		sm2_pub_key_verify		(SM2_PUB_KEY* pubk);
/* 根据私钥生成一些密钥扩展，用于加速后续的计算 */
extern	void	sm2_key_extend			(SM2_PRI_KEY_EXT* kext, SM2_PRI_KEY* prik);

/**
 * SM2公钥加密
*/

/**
 * 返回向c中输出的字节数，当返回值小于零时错误
*/
extern	int		sm2_encrypt	(SM2_PUB_KEY* key,uint8_t* m,int len,uint8_t* c);
/**
 * 返回向m中输出的字节数，当返回值小于零时错误
*/
extern	int		sm2_decrypt	(SM2_PRI_KEY* key,uint8_t* c,int len,uint8_t* m);

/**
 * SM2数字签名
*/

typedef struct _SM2_SIGNATURE{
	BN_256 r;
	BN_256 s;
}SM2_SIGNATURE;

/**
 * SM2数字签名的直接对象是用户的可辨识标识、椭圆曲线参数、公钥的杂凑值和待签名消息的杂凑值
 * 本API中，在进行签名/验签前，需要先计算该值
 * 在调用签名/验签API时，直接传入杂凑值即可
*/

extern	int		sm2_sig_init	(SM3_CTX* ctx, const SM2_PUB_KEY* key, uint8_t* id, int len);
extern	int		sm2_sig_update	(SM3_CTX* ctx, uint8_t* message, size_t len);
extern	void	sm2_sig_final	(SM3_CTX* ctx, uint8_t hash[32]);

extern	void	sm2_sig_generate(SM2_SIGNATURE* sig, SM2_PRI_KEY_EXT* key, uint8_t hash[32]);
extern	int		sm2_sig_verify	(SM2_SIGNATURE* sig, SM2_PUB_KEY* key, uint8_t hash[32]);

extern	void	sm2_sig_to_bin	(SM2_SIGNATURE* sig,uint8_t out[64]);
extern	void	sm2_sig_from_bin(SM2_SIGNATURE* sig,uint8_t in[64]);

__CPP_END

#endif
