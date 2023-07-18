#include <myy/algorithm.h>

int64_t gcd(int64_t a, int64_t b) {
	int64_t t;
	while(b!=0) {
		t=a%b;
		a=b;
		b=t;
	}
	return a;
}

int64_t exgcd(int64_t a, int64_t b, int64_t* x, int64_t* y){
	int64_t m=0, n=1, _x=1, _y=0, d, t;
	if(a<b) {
		t=a;a=b;b=t;
	}
	while(b!=0) {
		d=a/b;
		t=m; m=_x-d*t; _x=t;
		t=n; n=_y-d*t; _y=t;
		t=a%b; a=b; b=t;
	}
	*x=_x;*y=_y;
	return a;
}
