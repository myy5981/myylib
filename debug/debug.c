#include <stdio.h>
#include <string.h>

#include <sys/time.h>

#include "task.h"

int main(void){
	int i;
	struct timeval begin;
	struct timeval end;

	for(i=0;i<tasks_len;++i){
		Task* task=(Task*)tasks+i;
		if(task->before!=NULL){
			if(!task->before()){
				continue;
			}
		}
		if(task->task!=NULL){
			gettimeofday(&begin,NULL);
			task->task();
			gettimeofday(&end,NULL);
			printf("task%d cost:%ldus\n",i,(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);
		}
		if(task->after!=NULL){
			task->after();
		}
	}
	return 0;
}