#include <myy/algorithm.h>
#include <stdio.h>

int main(void){
    int64_t a,b,x,y,r;

    a=7612;b=321;

    r=exgcd(a,b,&x,&y);

    printf("r=%ld,x=%ld,y=%ld\n",r,x,y);
    return 0;
}