#include <myy/hex.h>

static const char* const upper="0123456789ABCDEFG";
static const char* const lower="0123456789abcdefg";

int hex_encode(void* src,size_t size,char* const dest,int flag){
	const char* map;
	if(flag==HEX_ENCODE_LOWER){
		map=lower;
	}else if(flag==HEX_ENCODE_UPPER){
		map=upper;
	}else return -1;
	size_t n=0;
	unsigned char* source=(unsigned char*)src;
	unsigned char e;
	for(size_t i=0;i<size;++i){
		e=source[i];
		dest[n++]=map[(e>>4)&0X0F];
		dest[n++]=map[e&0X0F];
	}
	return 0;
}

static inline unsigned char hex2byte(char hex){
	if(hex>='0'&&hex<='9'){
		return hex-'0';
	}
	if(hex>='A'&&hex<='F'){
		return hex-'A'+10;
	}
	if(hex>='a'&&hex<='f'){
		return hex-'a'+10;
	}
	return 16;
}

static size_t hex_decode_default(const char* src,size_t size,void* dest){
	size_t i=0,j=0;
	unsigned char e1,e2;
	while (i<size) {
		e1=hex2byte(src[i++]);
		if(e1>15){
			return -1;
		}
		e2=hex2byte(src[i++]);
		if(e2>15){
			return -1;
		}
		((unsigned char*)dest)[j++]=(e1<<4)|(e2&0x0F);
	}
	return j;
}

static size_t hex_decode_strict(const char* src,size_t size,void* dest){
	size_t i=0,j=0;
	char e;
	if((size&0x01)!=0){
		return -1;
	}
	while(i<size){
		e=src[i++];
		if(!((e>='0'&&e<='9')|(e>='a'&&e<='f')|(e>='A'&&e<='F'))){
			return -1;
		}
	}
	i=0;
	unsigned char e1,e2;
	while (i<size) {
		e1=hex2byte(src[i++]);
		e2=hex2byte(src[i++]);
		((unsigned char*)dest)[j++]=(e1<<4)|(e2&0x0F);
	}
	return j;
}

static size_t hex_decode_tolerant(const char* src,size_t size,void* dest){
	unsigned char e,c;
	int f=1;
	size_t i=0,j=0;
	while(i<size){
		c=hex2byte(src[i++]);
		if(c>15){
			continue;
		}
		if(f){
			e=c<<4;
			f=0;
		}else{
			e|=c;
			((unsigned char*)dest)[j++]=e;
			f=1;
		}
	}
	return j;
}

size_t hex_decode(const char* src,size_t len,void* dest,int flag){
	if(flag==HEX_DECODE_STRICT){
		return hex_decode_strict(src,len,dest);
	}
	if(flag==HEX_DECODE_DEFAULT){
		return hex_decode_default(src,len,dest);
	}
	if(flag==HEX_DECODE_TOLERANT){
		return hex_decode_tolerant(src,len,dest);
	}
	return -1;
}

void hex_enc2stream(FILE* fp,void* src,size_t len){
	char buf[1024];
	uint8_t* s=(uint8_t*)src;
	size_t i=0;
	int j=0,k=0;
	uint8_t e;
	for(i=0;i<len/512;++i){
		for(j=0;j<512;++j){
			e=s[j];
			buf[j*2]=lower[(e>>4)&0X0F];
			buf[j*2+1]=lower[e&0X0F];
		}
		s+=512;
		fwrite(buf,1,1024,fp);
	}
	j=len-i*512;
	for(k=0;k<j;++k){
		e=s[k];
		buf[k*2]=lower[(e>>4)&0X0F];
		buf[k*2+1]=lower[e&0X0F];
	}
	fwrite(buf,1,j*2,fp);
	fwrite("\n",1,1,fp);
}
