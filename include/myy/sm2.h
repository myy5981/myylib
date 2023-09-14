#ifndef MYY_SM2_H
#define MYY_SM2_H

#include <myy/env.h>
#include <myy/sm2_point.h>
#include <myy/sm3_kdf.h>

__CPP_BEGIN

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

/**
 * 将d作为私钥生存SM2密钥对
 * d可以为NULL，若d为NULL或d不符合SM2私钥条件，将会随机生成符合条件的密钥
*/
extern	void	sm2_key_generate		(SM2_PRI_KEY* prik, const BN_256 d);
extern	void	sm2_pub_generate		(SM2_PUB_KEY* pubk, const SM2_PRI_KEY* prik);
extern	int		sm2_pub_key_verify		(const SM2_PUB_KEY* pubk);
/* 根据私钥生成一些密钥扩展，用于加速后续的计算 */
extern	void	sm2_key_extend			(SM2_PRI_KEY_EXT* kext, const SM2_PRI_KEY* prik);

/**
 * SM2公钥加密
*/

/**
 * 返回向c中输出的字节数，当返回值小于零时错误
*/
extern	int		sm2_encrypt	(SM2_PUB_KEY* key, const uint8_t* m, int len, uint8_t* c);
/**
 * 返回向m中输出的字节数，当返回值小于零时错误
*/
extern	int		sm2_decrypt	(SM2_PRI_KEY* key, const uint8_t* c, int len, uint8_t* m);

/**
 * 根据用户的公钥和标识生成用户标识
 * id可以为NULL，若如此，id默认为"1234567812345678"，长度16字节，参数len被忽略
 * 该实现位于sm2_sig.c中
*/
extern	int		sm2_id_generate	(uint8_t hash[32], const SM2_PUB_KEY* key, const uint8_t* id, int len);

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

/**
 * id可以为NULL，若如此，id默认为"1234567812345678"，长度16字节，参数len被忽略
*/
extern	int		sm2_sig_init	(SM3_CTX* ctx, const SM2_PUB_KEY* key, const uint8_t* id, int len);
extern	int		sm2_sig_update	(SM3_CTX* ctx, const uint8_t* message, size_t len);
extern	void	sm2_sig_final	(SM3_CTX* ctx, uint8_t hash[32]);

extern	void	sm2_sig_generate(SM2_SIGNATURE* sig, const SM2_PRI_KEY_EXT* key, const uint8_t hash[32]);
extern	int		sm2_sig_verify	(const SM2_SIGNATURE* sig, const SM2_PUB_KEY* key, const uint8_t hash[32]);

extern	void	sm2_sig_to_bin	(const SM2_SIGNATURE* sig, uint8_t out[64]);
extern	void	sm2_sig_from_bin(SM2_SIGNATURE* sig, const uint8_t in[64]);

/**
 * SM2 密钥交换
*/

extern	int		sm2_ecmqv_init	(SM3_KDF_CTX* kdf,
									const SM2_PRI_KEY* k, const SM2_PRI_KEY* rk,
									const SM2_PUB_KEY* opposite_k, const SM2_PUB_KEY* opposite_rk,
									const uint8_t Z_cli[32], const uint8_t Z_srv[32]);

__CPP_END

#endif
