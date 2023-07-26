#include <myy/random.h>

/**
 * 先随便实现一个，这并不安全
*/
#include <stdlib.h>
#include <time.h>
void rand_bytes(uint8_t* r, size_t size){
	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < size; i++){
		r[i]=(uint8_t)rand();
	}
}