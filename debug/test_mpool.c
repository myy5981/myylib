#include "task.h"
#include <myy/mpool.h>
#include <stdlib.h>

void test_mpool_1(){
	MPOOL_1024B* pool = new_mpool_1024b(1024);
	for (size_t i = 0; i < 114514; i++)
	{
		mpool_alloc_1024b(pool);
	}
}

void test_malloc_1(){
	for (size_t i = 0; i < 114514; i++)
	{
		malloc(1024);
	}
}

const Task tasks[]={
	{.task=test_mpool_1,.after=NULL,.before=NULL},
	{.task=test_malloc_1,.after=NULL,.before=NULL}
};
int tasks_len=2;