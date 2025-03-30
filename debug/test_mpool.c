#include "task.h"
#include <myy/mpool.h>

void test_mpool_1(){
	MPOOL_1024B* pool = new_mpool_1024b(7);
	uint8_t* buf = (uint8_t*)mpool_alloc_1024b(pool);
	mpool_free_1024b(pool,buf);
}

const Task tasks[]={
	{.task=test_mpool_1,.after=NULL,.before=NULL}
};
int tasks_len=1;