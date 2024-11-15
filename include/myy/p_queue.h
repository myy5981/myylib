#ifndef MYY_PQUEUE_H
#define MYY_PQUEUE_H

#include <myy/env.h>

extern size_t pqueue_in(void* queue, size_t nitems, size_t size,const void* ele, int (*cmp)(const void *, const void *));
extern size_t pqueue_out(void* queue, size_t nitems, size_t size, void* ele, int (*cmp)(const void *, const void *));

#endif
