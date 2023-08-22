#include <myy/sm3.h>
#include <myy/endian.h>

#include <string.h>

static const uint32_t IV[]={0x7380166FU,0x4914B2B9U,0x172442D7U,0xDA8A0600U,0xA96F30BCU,0x163138AAU,0xE38DEE4DU,0xB0FB0E4EU};
// static const uint32_t T_0=0x79CC4519;
// static const uint32_t T_1=0x7A879D8A;
static const uint32_t T[64] = {
	0x79CC4519U,0xF3988A32U,0xE7311465U,0xCE6228CBU,
	0x9CC45197U,0x3988A32FU,0x7311465EU,0xE6228CBCU,
	0xCC451979U,0x988A32F3U,0x311465E7U,0x6228CBCEU,
	0xC451979CU,0x88A32F39U,0x11465E73U,0x228CBCE6U,
	0x9D8A7A87U,0x3B14F50FU,0x7629EA1EU,0xEC53D43CU,
	0xD8A7A879U,0xB14F50F3U,0x629EA1E7U,0xC53D43CEU,
	0x8A7A879DU,0x14F50F3BU,0x29EA1E76U,0x53D43CECU,
	0xA7A879D8U,0x4F50F3B1U,0x9EA1E762U,0x3D43CEC5U,
	0x7A879D8AU,0xF50F3B14U,0xEA1E7629U,0xD43CEC53U,
	0xA879D8A7U,0x50F3B14FU,0xA1E7629EU,0x43CEC53DU,
	0x879D8A7AU,0x0F3B14F5U,0x1E7629EAU,0x3CEC53D4U,
	0x79D8A7A8U,0xF3B14F50U,0xE7629EA1U,0xCEC53D43U,
	0x9D8A7A87U,0x3B14F50FU,0x7629EA1EU,0xEC53D43CU,
	0xD8A7A879U,0xB14F50F3U,0x629EA1E7U,0xC53D43CEU,
	0x8A7A879DU,0x14F50F3BU,0x29EA1E76U,0x53D43CECU,
	0xA7A879D8U,0x4F50F3B1U,0x9EA1E762U,0x3D43CEC5U,
};

#define FF_0(X,Y,Z) ((X)^(Y)^(Z))
#define FF_1(X,Y,Z) (((X)&(Y))|((X)&(Z))|((Y)&(Z)))
#define GG_0(X,Y,Z) ((X)^(Y)^(Z))
#define GG_1(X,Y,Z) (((X)&(Y))|(~(X)&(Z)))
#define L(X,b) (((X)<<(b))|((X)>>(32-(b))))
#define P_0(X) ((X)^L(X,9)^L(X,17))
#define P_1(X) ((X)^L(X,15)^L(X,23))

#define CF_1(A,B,C,D,E,F,G,H,x) \
	SS1=L((L(A,12)+E+T[i]), 7); \
	SS2=SS1 ^ L(A,12); \
	TT1=FF_##x(A,B,C)+D+SS2+(W[i]^W[i+4]); \
	TT2=GG_##x(E,F,G)+H+SS1+W[i]; \
	B=L(B,9); \
	H=TT1; \
	F=L(F,19); \
	D=P_0(TT2); \
	++i

#define CF_8(A,B,C,D,E,F,G,H,x) \
	CF_1(A,B,C,D,E,F,G,H,x); \
	CF_1(H,A,B,C,D,E,F,G,x); \
	CF_1(G,H,A,B,C,D,E,F,x); \
	CF_1(F,G,H,A,B,C,D,E,x); \
	CF_1(E,F,G,H,A,B,C,D,x); \
	CF_1(D,E,F,G,H,A,B,C,x); \
	CF_1(C,D,E,F,G,H,A,B,x); \
	CF_1(B,C,D,E,F,G,H,A,x)

#define CF(A,B,C,D,E,F,G,H) \
	CF_8(A,B,C,D,E,F,G,H,0); \
	CF_8(A,B,C,D,E,F,G,H,0); \
	CF_8(A,B,C,D,E,F,G,H,1); \
	CF_8(A,B,C,D,E,F,G,H,1); \
	CF_8(A,B,C,D,E,F,G,H,1); \
	CF_8(A,B,C,D,E,F,G,H,1); \
	CF_8(A,B,C,D,E,F,G,H,1); \
	CF_8(A,B,C,D,E,F,G,H,1)

void sm3_init(SM3_CTX* ctx){
	ctx->len=0;ctx->num=0;
	ctx->V[0]=IV[0];
	ctx->V[1]=IV[1];
	ctx->V[2]=IV[2];
	ctx->V[3]=IV[3];
	ctx->V[4]=IV[4];
	ctx->V[5]=IV[5];
	ctx->V[6]=IV[6];
	ctx->V[7]=IV[7];
}

static void sm3_update_block(SM3_CTX* ctx){
	uint32_t* W=ctx->W;
	uint32_t A=ctx->V[0];
	uint32_t B=ctx->V[1];
	uint32_t C=ctx->V[2];
	uint32_t D=ctx->V[3];
	uint32_t E=ctx->V[4];
	uint32_t F=ctx->V[5];
	uint32_t G=ctx->V[6];
	uint32_t H=ctx->V[7];
	uint32_t SS1,SS2,TT1,TT2;
	
	int i;
	uint32_t tmp;
	for(i=16;i<68;++i){
		tmp=W[i-16]^W[i-9]^L(W[i-3],15);
		W[i]=P_1(tmp)^L(W[i-13],7)^W[i-6];
	}
	i=0;
	
	CF(A,B,C,D,E,F,G,H);

	ctx->V[0]^=A;
	ctx->V[1]^=B;
	ctx->V[2]^=C;
	ctx->V[3]^=D;
	ctx->V[4]^=E;
	ctx->V[5]^=F;
	ctx->V[6]^=G;
	ctx->V[7]^=H;
}

/**
 * 对于dest，其中已经包含了num个有效的字节
 * 该函数的功能是从src中以区分大小端的方式，以四字节为单位,至多len个字节，拷贝到dest中，直至补齐64个字节
 * 返回新的num，0表示已经填满。
*/
static int memcpy_with_endian(uint32_t* dest,int num,uint8_t** src,size_t* len){
	uint8_t* _dest=((uint8_t*)dest)+num;
	uint8_t* _src=*src;
	size_t _len=*len;
	if(num+_len<SM3_BLOCK_LEN){
		memcpy(_dest,_src,_len);
		return num+_len;
	}
	int c_len=SM3_BLOCK_LEN-num;
	memcpy(_dest,_src,c_len);
	*src=_src+c_len;
	*len=_len-c_len;
	dest[0]=be2h_32(dest[0]);
	dest[1]=be2h_32(dest[1]);
	dest[2]=be2h_32(dest[2]);
	dest[3]=be2h_32(dest[3]);
	dest[4]=be2h_32(dest[4]);
	dest[5]=be2h_32(dest[5]);
	dest[6]=be2h_32(dest[6]);
	dest[7]=be2h_32(dest[7]);
	dest[8]=be2h_32(dest[8]);
	dest[9]=be2h_32(dest[9]);
	dest[10]=be2h_32(dest[10]);
	dest[11]=be2h_32(dest[11]);
	dest[12]=be2h_32(dest[12]);
	dest[13]=be2h_32(dest[13]);
	dest[14]=be2h_32(dest[14]);
	dest[15]=be2h_32(dest[15]);
	return 0;
}

int sm3_update(SM3_CTX* ctx, const uint8_t* data,size_t len){
	if(ctx->len+len>=SM3_MAX_LEN){
		return -1;
	}
	ctx->len=ctx->len+len;
	while((ctx->num=memcpy_with_endian(ctx->W,ctx->num,(uint8_t**)&data,&len))==0){
		sm3_update_block(ctx);
		if(len==0){
			break;
		}
	}
	return 0;
}

void sm3_final(SM3_CTX* ctx,void* dest){
	uint8_t* block=(uint8_t*)ctx->W;
	if(ctx->num>SM3_BLOCK_LEN-9){
		//两个块
		//第一个块
		block[ctx->num]=(uint8_t)0b10000000U;
		memset(block+ctx->num+1,0,SM3_BLOCK_LEN-1-ctx->num);
		for(int i=0;i<(ctx->num+1)/4+1;++i){
			ctx->W[i]=be2h_32(ctx->W[i]);
		}
		sm3_update_block(ctx);
		//第二个块
		memset(block,0,SM3_BLOCK_LEN-8);
		*(uint64_t*)(block+SM3_BLOCK_LEN-8)=h2be_64(ctx->len*8);
		ctx->W[14]=be2h_32(ctx->W[14]);
		ctx->W[15]=be2h_32(ctx->W[15]);
		sm3_update_block(ctx);
	}else{
		//一个块
		block[ctx->num]=(uint8_t)0b10000000U;
		memset(block+ctx->num+1,0,SM3_BLOCK_LEN-9-ctx->num);
		for(int i=0;i<(ctx->num+1)/4+1;++i){
			ctx->W[i]=be2h_32(ctx->W[i]);
		}
		*(uint64_t*)(block+SM3_BLOCK_LEN-8)=h2be_64(ctx->len*8);
		ctx->W[14]=be2h_32(ctx->W[14]);
		ctx->W[15]=be2h_32(ctx->W[15]);
		sm3_update_block(ctx);
	}
	((uint32_t*)dest)[0]=h2be_32(ctx->V[0]);
	((uint32_t*)dest)[1]=h2be_32(ctx->V[1]);
	((uint32_t*)dest)[2]=h2be_32(ctx->V[2]);
	((uint32_t*)dest)[3]=h2be_32(ctx->V[3]);
	((uint32_t*)dest)[4]=h2be_32(ctx->V[4]);
	((uint32_t*)dest)[5]=h2be_32(ctx->V[5]);
	((uint32_t*)dest)[6]=h2be_32(ctx->V[6]);
	((uint32_t*)dest)[7]=h2be_32(ctx->V[7]);
}

int sm3_hash(void* message,size_t len,void* dest){
	if(len>=SM3_MAX_LEN){
		return -1;
	}
	SM3_CTX ctx=SM3_CTX_STATIC_INIT;
	sm3_update(&ctx,(uint8_t*)message,len);
	sm3_final(&ctx,dest);
	return 0;
}

#define __CPY(F,i) dest->F[i]=src->F[i]
#define __CPY8(F,i) __CPY(F,i);  \
					__CPY(F,i+1);\
					__CPY(F,i+2);\
					__CPY(F,i+3);\
					__CPY(F,i+4);\
					__CPY(F,i+5);\
					__CPY(F,i+6);\
					__CPY(F,i+7)
void sm3_ctx_clone(SM3_CTX* src,SM3_CTX* dest){
	__CPY8(W,0); __CPY8(W,8);
	__CPY8(W,16);__CPY8(W,24);
	__CPY8(W,32);__CPY8(W,40);
	__CPY8(W,48);__CPY8(W,56);
	__CPY(W,64);__CPY(W,65);__CPY(W,66);__CPY(W,67);
	__CPY8(V,0);
	dest->len=src->len;
	dest->num=src->num;
}
#undef __CPY
#undef __CPY8
