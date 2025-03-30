#include "task.h"
#include <myy/mpool.h>

void test_mpool_1(){
	MPOOL_1024B* pool = new_mpool_1024b(2);
	uint8_t* buf1 = (uint8_t*)mpool_alloc_1024b(pool);
	uint8_t* buf2 = (uint8_t*)mpool_alloc_1024b(pool);
	uint8_t* buf3 = (uint8_t*)mpool_alloc_1024b(pool);
	uint8_t* buf4 = (uint8_t*)mpool_alloc_1024b(pool);
	uint8_t* buf5 = (uint8_t*)mpool_alloc_1024b(pool);
	mpool_free_1024b(pool,buf1);
	mpool_free_1024b(pool,buf5);
	mpool_free_1024b(pool,buf3);
	mpool_free_1024b(pool,buf2);
	mpool_free_1024b(pool,buf4);
}

const Task tasks[]={
	{.task=test_mpool_1,.after=NULL,.before=NULL}
};
int tasks_len=1;