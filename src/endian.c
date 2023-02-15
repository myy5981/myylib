#include <myy/endian.h>

uint16_t byte_swap_16(uint16_t x){
    return x<<8|x>>8;
}

uint32_t byte_swap_32(uint32_t x){
    return (x<<24)|((x<<8)&0x00FF0000U)|((x>>8)&0x0000FF00U)|(x>>24);
}

uint64_t byte_swap_64(uint64_t x){
    return  (x<<56)|
            ((x<<40)&0x00FF000000000000ULL)|
            ((x<<24)&0x0000FF0000000000ULL)|
            ((x<< 8)&0x000000FF00000000ULL)|
            ((x>> 8)&0x00000000FF000000ULL)|
            ((x>>24)&0x0000000000FF0000ULL)|
            ((x>>40)&0x000000000000FF00ULL)|
            (x>>56);

}
