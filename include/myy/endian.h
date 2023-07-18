#ifndef MYY_ENDIAN_H
#define MYY_ENDIAN_H

#include <myy/env.h>

#define	__byte_swap_16(x) ((((uint16_t)(x))<<8)|(((uint16_t)(x))>>8))

#define	__byte_swap_32(x) (			\
(((uint32_t)(x))<<24)|				\
((((uint32_t)(x))<<8)&0x00FF0000U)|	\
((((uint32_t)(x))>>8)&0x0000FF00U)|	\
(((uint32_t)(x))>>24))

#define	__byte_swap_64(x) (					 	\
(((uint64_t)(x))<<56)|						  	\
((((uint64_t)(x))<<40)&0x00FF000000000000ULL)|	\
((((uint64_t)(x))<<24)&0x0000FF0000000000ULL)|	\
((((uint64_t)(x))<< 8)&0x000000FF00000000ULL)|	\
((((uint64_t)(x))>> 8)&0x00000000FF000000ULL)|	\
((((uint64_t)(x))>>24)&0x0000000000FF0000ULL)|	\
((((uint64_t)(x))>>40)&0x000000000000FF00ULL)|	\
(((uint64_t)(x))>>56))

#if __BYTE_ORDER == __LITTLE_ENDIAN
	#define SYS_LITTLE_ENDIAN 1

	/* host to little endian */
	#define h2le_16(x) (x)
	#define h2be_16(x) __byte_swap_16(x)
	#define le2h_16(x) (x)
	#define be2h_16(x) __byte_swap_16(x)

	#define h2le_32(x) (x)
	#define h2be_32(x) __byte_swap_32(x)
	#define le2h_32(x) (x)
	#define be2h_32(x) __byte_swap_32(x)

	#define h2le_64(x) (x)
	#define h2be_64(x) __byte_swap_64(x)
	#define le2h_64(x) (x)
	#define be2h_64(x) __byte_swap_64(x)

#elif __BYTE_ORDER == __BIG_ENDIAN
	#define SYS_BIG_ENDIAN 1

	/* host to big endian */
	#define h2be_16(x) (x)
	#define h2le_16(x) __byte_swap_16(x)
	#define be2h_16(x) (x)
	#define le2h_16(x) __byte_swap_16(x)

	#define h2be_32(x) (x)
	#define h2le_32(x) __byte_swap_32(x)
	#define be2h_32(x) (x)
	#define le2h_32(x) __byte_swap_32(x)

	#define h2be_64(x) (x)
	#define h2le_64(x) __byte_swap_64(x)
	#define be2h_64(x) (x)
	#define le2h_64(x) __byte_swap_64(x)

#else
	#error "Unsupported System Byte Order"
#endif

__CPP_BEGIN

extern uint16_t byte_swap_16(uint16_t x);
extern uint32_t byte_swap_32(uint32_t x);
extern uint64_t byte_swap_64(uint64_t x);

__CPP_END

#endif
