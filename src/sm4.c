#include <myy/sm4.h>
#include <myy/endian.h>

#define ROTL(X,b) (((X)<<(b))|((X)>>(32-(b))))
#define B4(W) ((W)&0x000000FFu)
#define B3(W) (((W)&0x0000FF00u)>>8)
#define B2(W) (((W)&0x00FF0000u)>>16)
#define B1(W) (((W)&0xFF000000u)>>24)
#define tao(A) (Sbox[B1(A)]<<24)|(Sbox[B2(A)]<<16)|(Sbox[B3(A)]<<8)|(Sbox[B4(A)])
#define L_ENC(B) ((B)^ROTL(B,2)^ROTL(B,10)^ROTL(B,18)^ROTL(B,24))
#define L_KEY(B) ((B)^ROTL(B,13)^ROTL(B,23))

#define FK_0 0xA3B1BAC6u
#define FK_1 0x56AA3350u
#define FK_2 0x677D9197u
#define FK_3 0XB27022DCu

#define CK_0  0x00070E15u
#define CK_1  0x1C232A31u
#define CK_2  0x383F464Du
#define CK_3  0x545B6269u
#define CK_4  0x70777E85u
#define CK_5  0x8C939AA1u
#define CK_6  0xA8AFB6BDu
#define CK_7  0xC4CBD2D9u
#define CK_8  0xE0E7EEF5u
#define CK_9  0xFC030A11u
#define CK_10 0x181F262Du
#define CK_11 0x343B4249u
#define CK_12 0x50575E65u
#define CK_13 0x6C737A81u
#define CK_14 0x888F969Du
#define CK_15 0xA4ABB2B9u
#define CK_16 0xC0C7CED5u
#define CK_17 0xDCE3EAF1u
#define CK_18 0xF8FF060Du
#define CK_19 0x141B2229u
#define CK_20 0x30373E45u
#define CK_21 0x4C535A61u
#define CK_22 0x686F767Du
#define CK_23 0x848B9299u
#define CK_24 0xA0A7AEB5u
#define CK_25 0xBCC3CAD1u
#define CK_26 0xD8DFE6EDu
#define CK_27 0xF4FB0209u
#define CK_28 0x10171E25u
#define CK_29 0x2C333A41u
#define CK_30 0x484F565Du
#define CK_31 0x646B7279u

static const uint8_t Sbox[256]={
	0xD6u,0x90u,0xE9u,0xFEu,0xCCu,0xE1u,0x3Du,0xB7u,0x16u,0xB6u,0x14u,0xC2u,0x28u,0xFBu,0x2Cu,0x05u,
	0x2Bu,0x67u,0x9Au,0x76u,0x2Au,0xBEu,0x04u,0xC3u,0xAAu,0x44u,0x13u,0x26u,0x49u,0x86u,0x06u,0x99u,
	0x9Cu,0x42u,0x50u,0xF4u,0x91u,0xEFu,0x98u,0x7Au,0x33u,0x54u,0x0Bu,0x43u,0xEDu,0xCFu,0xACu,0x62u,
	0xE4u,0xB3u,0x1Cu,0xA9u,0xC9u,0x08u,0xE8u,0x95u,0x80u,0xDFu,0x94u,0xFAu,0x75u,0x8Fu,0x3Fu,0xA6u,
	0x47u,0x07u,0xA7u,0xFCu,0xF3u,0x73u,0x17u,0xBAu,0x83u,0x59u,0x3Cu,0x19u,0xE6u,0x85u,0x4Fu,0xA8u,
	0x68u,0x6Bu,0x81u,0xB2u,0x71u,0x64u,0xDAu,0x8Bu,0xF8u,0xEBu,0x0Fu,0x4Bu,0x70u,0x56u,0x9Du,0x35u,
	0x1Eu,0x24u,0x0Eu,0x5Eu,0x63u,0x58u,0xD1u,0xA2u,0x25u,0x22u,0x7Cu,0x3Bu,0x01u,0x21u,0x78u,0x87u,
	0xD4u,0x00u,0x46u,0x57u,0x9Fu,0xD3u,0x27u,0x52u,0x4Cu,0x36u,0x02u,0xE7u,0xA0u,0xC4u,0xC8u,0x9Eu,
	0xEAu,0xBFu,0x8Au,0xD2u,0x40u,0xC7u,0x38u,0xB5u,0xA3u,0xF7u,0xF2u,0xCEu,0xF9u,0x61u,0x15u,0xA1u,
	0xE0u,0xAEu,0x5Du,0xA4u,0x9Bu,0x34u,0x1Au,0x55u,0xADu,0x93u,0x32u,0x30u,0xF5u,0x8Cu,0xB1u,0xE3u,
	0x1Du,0xF6u,0xE2u,0x2Eu,0x82u,0x66u,0xCAu,0x60u,0xC0u,0x29u,0x23u,0xABu,0x0Du,0x53u,0x4Eu,0x6Fu,
	0xD5u,0xDBu,0x37u,0x45u,0xDEu,0xFDu,0x8Eu,0x2Fu,0x03u,0xFFu,0x6Au,0x72u,0x6Du,0x6Cu,0x5Bu,0x51u,
	0x8Du,0x1Bu,0xAFu,0x92u,0xBBu,0xDDu,0xBCu,0x7Fu,0x11u,0xD9u,0x5Cu,0x41u,0x1Fu,0x10u,0x5Au,0xD8u,
	0x0Au,0xC1u,0x31u,0x88u,0xA5u,0xCDu,0x7Bu,0xBDu,0x2Du,0x74u,0xD0u,0x12u,0xB8u,0xE5u,0xB4u,0xB0u,
	0x89u,0x69u,0x97u,0x4Au,0x0Cu,0x96u,0x77u,0x7Eu,0x65u,0xB9u,0xF1u,0x09u,0xC5u,0x6Eu,0xC6u,0x84u,
	0x18u,0xF0u,0x7Du,0xECu,0x3Au,0xDCu,0x4Du,0x20u,0x79u,0xEEu,0x5Fu,0x3Eu,0xD7u,0xCBu,0x39u,0x48u,
};

/* 合成置换T */
#define T(x1,x2,x3,k,xx) \
		t=(x1)^(x2)^(x3)^(k);\
		t=tao(t);\
		t=L_##xx(t)

/* 轮函数F */
#define F(x0,x1,x2,x3,rk) \
		T(x1,x2,x3,rk,ENC); \
		x0^=t;

/* 密钥扩展函数 */
#define EXT(x0,x1,x2,x3,xx) \
		T(x1,x2,x3,CK_##xx,KEY); \
		x0^=t; \
		key[xx]=x0

void sm4_key_init(SM4_KEY key, uint8_t origin[16]){
	uint32_t* o=(uint32_t*)origin;
	uint32_t k0=be2h_32(o[0])^FK_0;
	uint32_t k1=be2h_32(o[1])^FK_1;
	uint32_t k2=be2h_32(o[2])^FK_2;
	uint32_t k3=be2h_32(o[3])^FK_3;
	uint32_t t;
	EXT(k0,k1,k2,k3, 0); EXT(k1,k2,k3,k0, 1); EXT(k2,k3,k0,k1, 2); EXT(k3,k0,k1,k2, 3);
	EXT(k0,k1,k2,k3, 4); EXT(k1,k2,k3,k0, 5); EXT(k2,k3,k0,k1, 6); EXT(k3,k0,k1,k2, 7);
	EXT(k0,k1,k2,k3, 8); EXT(k1,k2,k3,k0, 9); EXT(k2,k3,k0,k1,10); EXT(k3,k0,k1,k2,11);
	EXT(k0,k1,k2,k3,12); EXT(k1,k2,k3,k0,13); EXT(k2,k3,k0,k1,14); EXT(k3,k0,k1,k2,15);
	EXT(k0,k1,k2,k3,16); EXT(k1,k2,k3,k0,17); EXT(k2,k3,k0,k1,18); EXT(k3,k0,k1,k2,19);
	EXT(k0,k1,k2,k3,20); EXT(k1,k2,k3,k0,21); EXT(k2,k3,k0,k1,22); EXT(k3,k0,k1,k2,23);
	EXT(k0,k1,k2,k3,24); EXT(k1,k2,k3,k0,25); EXT(k2,k3,k0,k1,26); EXT(k3,k0,k1,k2,27);
	EXT(k0,k1,k2,k3,28); EXT(k1,k2,k3,k0,29); EXT(k2,k3,k0,k1,30); EXT(k3,k0,k1,k2,31);
}

void sm4_encrypt_block(SM4_KEY key,uint8_t message[16],uint8_t cipher[16]){
	uint32_t* _m=(uint32_t*)message;
	uint32_t* _c=(uint32_t*)cipher;
	uint32_t x0=be2h_32(_m[0]);
	uint32_t x1=be2h_32(_m[1]);
	uint32_t x2=be2h_32(_m[2]);
	uint32_t x3=be2h_32(_m[3]);
	uint32_t t;
	F(x0,x1,x2,x3,key[ 0]); F(x1,x2,x3,x0,key[ 1]); F(x2,x3,x0,x1,key[ 2]); F(x3,x0,x1,x2,key[ 3]);
	F(x0,x1,x2,x3,key[ 4]); F(x1,x2,x3,x0,key[ 5]); F(x2,x3,x0,x1,key[ 6]); F(x3,x0,x1,x2,key[ 7]);
	F(x0,x1,x2,x3,key[ 8]); F(x1,x2,x3,x0,key[ 9]); F(x2,x3,x0,x1,key[10]); F(x3,x0,x1,x2,key[11]);
	F(x0,x1,x2,x3,key[12]); F(x1,x2,x3,x0,key[13]); F(x2,x3,x0,x1,key[14]); F(x3,x0,x1,x2,key[15]);
	F(x0,x1,x2,x3,key[16]); F(x1,x2,x3,x0,key[17]); F(x2,x3,x0,x1,key[18]); F(x3,x0,x1,x2,key[19]);
	F(x0,x1,x2,x3,key[20]); F(x1,x2,x3,x0,key[21]); F(x2,x3,x0,x1,key[22]); F(x3,x0,x1,x2,key[23]);
	F(x0,x1,x2,x3,key[24]); F(x1,x2,x3,x0,key[25]); F(x2,x3,x0,x1,key[26]); F(x3,x0,x1,x2,key[27]);
	F(x0,x1,x2,x3,key[28]); F(x1,x2,x3,x0,key[29]); F(x2,x3,x0,x1,key[30]); F(x3,x0,x1,x2,key[31]);
	_c[0]=h2be_32(x3);
	_c[1]=h2be_32(x2);
	_c[2]=h2be_32(x1);
	_c[3]=h2be_32(x0);
}

void sm4_encrypt_block_with_xor(SM4_KEY key,uint8_t message[16],uint8_t perfix[16],uint8_t cipher[16]){
	uint32_t* _m=(uint32_t*)message;
	uint32_t* _m_=(uint32_t*)perfix;
	uint32_t* _c=(uint32_t*)cipher;
	uint32_t x0=be2h_32(_m[0]^_m_[0]);
	uint32_t x1=be2h_32(_m[1]^_m_[1]);
	uint32_t x2=be2h_32(_m[2]^_m_[2]);
	uint32_t x3=be2h_32(_m[3]^_m_[3]);
	uint32_t t;
	F(x0,x1,x2,x3,key[ 0]); F(x1,x2,x3,x0,key[ 1]); F(x2,x3,x0,x1,key[ 2]); F(x3,x0,x1,x2,key[ 3]);
	F(x0,x1,x2,x3,key[ 4]); F(x1,x2,x3,x0,key[ 5]); F(x2,x3,x0,x1,key[ 6]); F(x3,x0,x1,x2,key[ 7]);
	F(x0,x1,x2,x3,key[ 8]); F(x1,x2,x3,x0,key[ 9]); F(x2,x3,x0,x1,key[10]); F(x3,x0,x1,x2,key[11]);
	F(x0,x1,x2,x3,key[12]); F(x1,x2,x3,x0,key[13]); F(x2,x3,x0,x1,key[14]); F(x3,x0,x1,x2,key[15]);
	F(x0,x1,x2,x3,key[16]); F(x1,x2,x3,x0,key[17]); F(x2,x3,x0,x1,key[18]); F(x3,x0,x1,x2,key[19]);
	F(x0,x1,x2,x3,key[20]); F(x1,x2,x3,x0,key[21]); F(x2,x3,x0,x1,key[22]); F(x3,x0,x1,x2,key[23]);
	F(x0,x1,x2,x3,key[24]); F(x1,x2,x3,x0,key[25]); F(x2,x3,x0,x1,key[26]); F(x3,x0,x1,x2,key[27]);
	F(x0,x1,x2,x3,key[28]); F(x1,x2,x3,x0,key[29]); F(x2,x3,x0,x1,key[30]); F(x3,x0,x1,x2,key[31]);
	_c[0]=h2be_32(x3);
	_c[1]=h2be_32(x2);
	_c[2]=h2be_32(x1);
	_c[3]=h2be_32(x0);
}

void sm4_decrypt_block(SM4_KEY key,uint8_t cipher[16],uint8_t message[16]){
	uint32_t* _c=(uint32_t*)cipher;
	uint32_t* _m=(uint32_t*)message;
	uint32_t x0=be2h_32(_c[0]);
	uint32_t x1=be2h_32(_c[1]);
	uint32_t x2=be2h_32(_c[2]);
	uint32_t x3=be2h_32(_c[3]);
	uint32_t t;
	F(x0,x1,x2,x3,key[31]); F(x1,x2,x3,x0,key[30]); F(x2,x3,x0,x1,key[29]); F(x3,x0,x1,x2,key[28]);
	F(x0,x1,x2,x3,key[27]); F(x1,x2,x3,x0,key[26]); F(x2,x3,x0,x1,key[25]); F(x3,x0,x1,x2,key[24]);
	F(x0,x1,x2,x3,key[23]); F(x1,x2,x3,x0,key[22]); F(x2,x3,x0,x1,key[21]); F(x3,x0,x1,x2,key[20]);
	F(x0,x1,x2,x3,key[19]); F(x1,x2,x3,x0,key[18]); F(x2,x3,x0,x1,key[17]); F(x3,x0,x1,x2,key[16]);
	F(x0,x1,x2,x3,key[15]); F(x1,x2,x3,x0,key[14]); F(x2,x3,x0,x1,key[13]); F(x3,x0,x1,x2,key[12]);
	F(x0,x1,x2,x3,key[11]); F(x1,x2,x3,x0,key[10]); F(x2,x3,x0,x1,key[ 9]); F(x3,x0,x1,x2,key[ 8]);
	F(x0,x1,x2,x3,key[ 7]); F(x1,x2,x3,x0,key[ 6]); F(x2,x3,x0,x1,key[ 5]); F(x3,x0,x1,x2,key[ 4]);
	F(x0,x1,x2,x3,key[ 3]); F(x1,x2,x3,x0,key[ 2]); F(x2,x3,x0,x1,key[ 1]); F(x3,x0,x1,x2,key[ 0]);
	_m[0]=h2be_32(x3);
	_m[1]=h2be_32(x2);
	_m[2]=h2be_32(x1);
	_m[3]=h2be_32(x0);
}

void sm4_decrypt_block_with_xor(SM4_KEY key,uint8_t cipher[16],uint8_t perfix[16],uint8_t message[16]){
	uint32_t* _c=(uint32_t*)cipher;
	uint32_t* _c_=(uint32_t*)perfix;
	uint32_t* _m=(uint32_t*)message;
	uint32_t x0=be2h_32(_c[0]);
	uint32_t x1=be2h_32(_c[1]);
	uint32_t x2=be2h_32(_c[2]);
	uint32_t x3=be2h_32(_c[3]);
	uint32_t t;
	F(x0,x1,x2,x3,key[31]); F(x1,x2,x3,x0,key[30]); F(x2,x3,x0,x1,key[29]); F(x3,x0,x1,x2,key[28]);
	F(x0,x1,x2,x3,key[27]); F(x1,x2,x3,x0,key[26]); F(x2,x3,x0,x1,key[25]); F(x3,x0,x1,x2,key[24]);
	F(x0,x1,x2,x3,key[23]); F(x1,x2,x3,x0,key[22]); F(x2,x3,x0,x1,key[21]); F(x3,x0,x1,x2,key[20]);
	F(x0,x1,x2,x3,key[19]); F(x1,x2,x3,x0,key[18]); F(x2,x3,x0,x1,key[17]); F(x3,x0,x1,x2,key[16]);
	F(x0,x1,x2,x3,key[15]); F(x1,x2,x3,x0,key[14]); F(x2,x3,x0,x1,key[13]); F(x3,x0,x1,x2,key[12]);
	F(x0,x1,x2,x3,key[11]); F(x1,x2,x3,x0,key[10]); F(x2,x3,x0,x1,key[ 9]); F(x3,x0,x1,x2,key[ 8]);
	F(x0,x1,x2,x3,key[ 7]); F(x1,x2,x3,x0,key[ 6]); F(x2,x3,x0,x1,key[ 5]); F(x3,x0,x1,x2,key[ 4]);
	F(x0,x1,x2,x3,key[ 3]); F(x1,x2,x3,x0,key[ 2]); F(x2,x3,x0,x1,key[ 1]); F(x3,x0,x1,x2,key[ 0]);
	_m[0]=h2be_32(x3)^_c_[0];
	_m[1]=h2be_32(x2)^_c_[1];
	_m[2]=h2be_32(x1)^_c_[2];
	_m[3]=h2be_32(x0)^_c_[3];
}
