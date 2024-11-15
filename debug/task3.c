#include <stdio.h>
#include <myy/p_queue.h>
#include "task.h"

int intcmp(const void* a,const void* b){
    return *(int*)a - *(int*)b;
}

void run(){
    int data[100]={0};
    int n = 0;
    int ele = 2;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 5;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 7;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 3;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 6;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 5;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 9;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 0;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 2;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 4;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 8;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 1;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 0;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 9;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 3;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 6;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 4;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 7;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);
    ele = 1;
    n=pqueue_in(data,n,sizeof(int),&ele,intcmp);

    for (size_t i = 0; i < n; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");
    while (n!=0)
    {
        n=pqueue_out(data,n,sizeof(int),&ele,intcmp);
        printf("%d ",ele);
    }
    
    
    return;
}

const Task tasks[]={
	{.before=NULL,.task=run,.after=NULL}
};
int tasks_len=1;
