#ifndef MYY_BIGNUM_H 
#define MYY_BIGNUM_H

#include <myy/env.h>

__CPP_BEGIN

//大端存储，高32位存在低索引，低32位存在高索引
#define	BN_256_LEN	8
#define	BN_288_LEN	9
#define	BN_512_LEN	16

typedef	uint32_t	BN_UNIT;
typedef	uint32_t	BN_256[BN_256_LEN];
typedef	uint32_t	BN_288[BN_288_LEN];
typedef	uint32_t	BN_512[BN_512_LEN];
typedef	uint32_t*	BN_256_ptr;
typedef	uint32_t*	BN_288_ptr;
typedef	uint32_t*	BN_512_ptr;

#define	BN_UNIT_LEN	32

/**
 * 以下宏用于大数的静态初始化
*/

#define	BN_256_INIT(a,b,c,d,e,f,g,h)					{0x##a##U,0x##b##U,0x##c##U,0x##d##U,0x##e##U,0x##f##U,0x##g##U,0x##h##U}
#define	BN_512_INIT(A,B,C,D,E,F,G,H,a,b,c,d,e,f,g,h)	{0x##A##U,0x##B##U,0x##C##U,0x##D##U,0x##E##U,0x##F##U,0x##G##U,0x##H##U,0x##a##U,0x##b##U,0x##c##U,0x##d##U,0x##e##U,0x##f##U,0x##g##U,0x##h##U}
#define	BN_256_ZERO										{0,0,0,0,0,0,0,0}
#define	BN_256_ONE										{0,0,0,0,0,0,0,1}
#define	BN_256_TWO										{0,0,0,0,0,0,0,2}

/**
 * 以下是在大数中节取部分的宏，注意使用共享的内存
*/

#define	bn_512_l256(a)	(BN_256_ptr)(a+8)
#define	bn_512_l288(a)	(BN_288_ptr)(a+9)
#define	bn_512_h256(a)	(BN_256_ptr)(a)

/**
 * 以下是256位整数的功能函数或宏定义
*/

#define	bn_256_is_zero(n)		(((n)[0]==0)&&((n)[1]==0)&&((n)[2]==0)&&((n)[3]==0)&&((n)[4]==0)&&((n)[5]==0)&&((n)[6]==0)&&((n)[7]==0))
#define	bn_256_is_one(n)		(((n)[0]==0)&&((n)[1]==0)&&((n)[2]==0)&&((n)[3]==0)&&((n)[4]==0)&&((n)[5]==0)&&((n)[6]==0)&&((n)[7]==1))
#define	bn_256_is_odd(n)		(n[7]&0x01)
/* 如果发生进位则返回1，否则返回0 */
extern	int		bn_256_adc		(BN_256 r, const BN_256 a, const BN_256 b);
extern	void	bn_256_add		(BN_256 r, const BN_256 a, const BN_256 b);
extern	void	bn_256_inc		(BN_256 a, const BN_256 b);
extern	int		bn_256_inc1		(BN_256 a);
extern	void	bn_256_sub		(BN_256 r, const BN_256 a, const BN_256 b);
extern	void	bn_256_mul		(BN_512 r, const BN_256 a, const BN_256 b);
extern	void	bn_256_imul		(BN_256 r, const BN_256 a, const BN_256 b);
extern	int		bn_256_cmp		(const BN_256 a, const BN_256 b);
extern	void	bn_256_set_zero	(BN_256 bn);
extern	void	bn_256_set_word	(BN_256 bn, uint32_t w);
extern	void	bn_256_to_bin	(const BN_256 bn, uint8_t* dest);
extern	void	bn_256_from_bin	(BN_256 bn, const uint8_t* src);
extern	void	bn_256_cpy		(BN_256 r, const BN_256 a);

#ifdef __MYY_PRIVATE_API__
/**
 * 以下是为了节省指令使用的异常不文明不优雅的内部接口，不推荐使用
*/

extern	void	__bn_288_sub		(BN_512 r, const BN_512 a, const BN_512 b);
extern	void	__bn_288_sbb_256	(BN_512 r, const BN_256 b);
extern	int		__bn_288_cmp_256	(const BN_512 a, const BN_256 b);
extern	int		__bn_512_h256_adc	(BN_256 r, const BN_512 a, const BN_512 b);

#endif

__CPP_END

#endif