#include <stdlib.h>
#include <sys/mman.h>

#include <myy/mpool.h>

MPOOL_1024B* new_mpool_1024b(uint32_t capacity) {
	if (capacity >= 65536) {
		capacity = 65536;
	} else if (capacity == 0) {
		capacity = 1;
	} else {
		capacity--;
		capacity |= capacity >> 1;
		capacity |= capacity >> 2;
		capacity |= capacity >> 4;
		capacity |= capacity >> 8;
		capacity++;
	}
	MPOOL_1024B* pool = (MPOOL_1024B*)malloc(sizeof(MPOOL_1024B));
	if (pool == NULL) {
		return NULL;
	}
	pool->root.free = (uint16_t*)malloc(sizeof(uint16_t) * capacity);
	if (pool->root.free == NULL) {
		free(pool);
		return NULL;
	}
	pool->root.m = mmap(NULL, 1024 * capacity, PROT_READ | PROT_WRITE,
						MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (pool->root.m == MAP_FAILED) {
		free(pool->root.free);
		free(pool);
		return NULL;
	}
	pool->root.capacity = capacity;
	pool->root.idx = capacity;
	for (int i = 0; i < capacity; ++i) {
		pool->root.free[i] = capacity - 1 - i;
	}
	pool->root.next_chunk = NULL;
	return pool;
}

static MCHUNK_1024B* new_mchunk_1024b(uint32_t capacity) {
	// if (capacity >= 65536) {
	// 	capacity = 65536;
	// } else if (capacity == 0) {
	// 	capacity = 1;
	// } else {
	// 	capacity--;
	// 	capacity |= capacity >> 1;
	// 	capacity |= capacity >> 2;
	// 	capacity |= capacity >> 4;
	// 	capacity |= capacity >> 8;
	// 	capacity++;
	// }
	MCHUNK_1024B* chunk = (MCHUNK_1024B*)malloc(sizeof(MCHUNK_1024B));
	if (chunk == NULL) {
		return NULL;
	}
	chunk->free = (uint16_t*)malloc(sizeof(uint16_t) * capacity);
	if (chunk->free == NULL) {
		free(chunk);
		return NULL;
	}
	chunk->m = mmap(NULL, 1024 * capacity, PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (chunk->m == MAP_FAILED) {
		free(chunk->free);
		free(chunk);
		return NULL;
	}
	chunk->capacity = capacity;
	chunk->idx = capacity;
	for (int i = 0; i < capacity; ++i) {
		chunk->free[i] = capacity - 1 - i;
	}
	chunk->next_chunk = NULL;
	return chunk;
}

static void mchunk_free_1024b(MCHUNK_1024B* chunk) {
	free(chunk->free);
	munmap(chunk->m, chunk->capacity * 1024);
}

void* mpool_alloc_1024b(MPOOL_1024B* pool) {
	if (pool == NULL) {
		return NULL;
	}
	MCHUNK_1024B* chunk = &(pool->root);
	while (chunk->idx == 0) {
		// sole out
		if (chunk->next_chunk == NULL) {
			chunk->next_chunk = new_mchunk_1024b(chunk->capacity);
			if (chunk->next_chunk == NULL) {
				return NULL;
			}
		}
		chunk = chunk->next_chunk;
	}
	if (chunk == pool->free_chunk) {
		pool->free_chunk = NULL;
	}
	uint32_t idx = chunk->free[--chunk->idx];
	return chunk->m + (1024 * idx);
}

void mpool_free_1024b(MPOOL_1024B* pool, void* ptr) {
	if (pool == NULL) {
		return;
	}
	if ((uint64_t)ptr % 1024 != 0) {
		return;
	}
	MCHUNK_1024B* chunk = &(pool->root);
	MCHUNK_1024B* prv = NULL;
	while (chunk != NULL) {
		if (((uint64_t)ptr ^ (uint64_t)chunk->m) &
			(0xFFFFFFFFFFFFFFFF * chunk->capacity * 1024)) {
			prv = chunk;
			chunk = chunk->next_chunk;
			continue;
		}
		uint16_t idx = (uint16_t)(((uint64_t)ptr - (uint64_t)chunk->m) / 1024);
		chunk->free[chunk->idx++] = idx;
		if (chunk->idx == chunk->capacity) {  // 当内存池全部空闲时
			if (chunk != &(pool->root)) {	  // 如果不是第一块chunk
				if (pool->free_chunk != NULL) {
					// 如果有上一个未释放的chunk，把上一个的释放掉
					pool->free_chunk_prv->next_chunk = pool->free_chunk->next_chunk;
					mchunk_free_1024b(pool->free_chunk);
				}
				pool->free_chunk = chunk;
				pool->free_chunk_prv = prv;
			}
		}
		return;
	}
}

void destory_mpool_1024b(MPOOL_1024B* pool) {
	if (pool != NULL) {
		MCHUNK_1024B* chunk = &(pool->root);
		while (chunk != NULL) {
			MCHUNK_1024B* next = chunk->next_chunk;
			mchunk_free_1024b(chunk);
			chunk = next;
		}
		free(pool);
	}
}