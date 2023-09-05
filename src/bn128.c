#include <myy/bn128.h>

void bn_128_gf_rshift1(BN_128 r, BN_128 a){
	r[1]=(a[0]<<63)|(a[1]>>1);
	r[0]=a[0]>>1;
}

void bn_128_gf_add(BN_128 r, BN_128 a, BN_128 b){
	r[0]=a[0]^b[0];
	r[1]=a[1]^b[1];
}

void bn_128_gf_mul(BN_128 r, BN_128 a, BN_128 b){
	BN_128 v;
	v[0]=b[0];v[1]=b[1];
	r[0]=0;r[1]=0;
	for(int i=0;i<64;++i){
		if((a[1]>>i)&0x01ULL){
			bn_128_gf_add(r,r,v);
		}
		if(v[1]&0x01ULL){
			bn_128_gf_rshift1(v,v);
			v[0]^=0xE100000000000000ULL;
		}else{
			bn_128_gf_rshift1(v,v);
		}
	}
	for(int i=0;i<64;++i){
		if((a[0]>>i)&0x01ULL){
			bn_128_gf_add(r,r,v);
		}
		if(v[1]&0x01ULL){
			bn_128_gf_rshift1(v,v);
			v[0]^=0xE100000000000000ULL;
		}else{
			bn_128_gf_rshift1(v,v);
		}
	}
}