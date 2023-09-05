#include <myy/random.h>

/**
 * 先随便实现一个，这并不安全
*/
#include <sys/random.h>
#include <stdlib.h>
#include <time.h>
void rand_bytes(uint8_t* r, size_t size){
	if(getrandom(r,size,GRND_NONBLOCK)<0){
		exit(0);
	}
}