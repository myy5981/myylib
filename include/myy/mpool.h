#ifndef MYY_MPOOL_H
#define MYY_MPOOL_H

#include <myy/env.h>

struct _MCHUNK_1024B {
	void* m;
	struct _MCHUNK_1024B* next_chunk;
	uint16_t* free;
	uint32_t idx;
	uint32_t capacity;
};
typedef struct _MCHUNK_1024B MCHUNK_1024B;

struct _MPOOL_1024B {
	MCHUNK_1024B root;
	MCHUNK_1024B* free_chunk;
	MCHUNK_1024B* free_chunk_prv;
};

typedef struct _MPOOL_1024B MPOOL_1024B;

extern	MPOOL_1024B*	new_mpool_1024b		(uint32_t capacity);
extern	void*			mpool_alloc_1024b	(MPOOL_1024B* pool);
extern	void			mpool_free_1024b	(MPOOL_1024B* pool, void* ptr);
extern	void			destory_mpool_1024b	(MPOOL_1024B* pool);

#endif
