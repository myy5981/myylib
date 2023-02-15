#include <myy/url_base64.h>

static const char* const base_map="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

size_t url_base64_encode(const void* src,size_t len,char* dest){
    size_t l=len/3;
    size_t r=len-l*3;
    size_t i=0,j=0;
    struct cell {uint8_t e1,e2,e3;} c;
    for(;i<l;++i){
        c=((struct cell*)src)[i];
        dest[j++]=base_map[c.e1>>2];
        dest[j++]=base_map[((c.e1<<4)|(c.e2>>4))&0x3F];
        dest[j++]=base_map[((c.e2<<2)|(c.e3>>6))&0x3F];
        dest[j++]=base_map[c.e3&0x3F];
    }
    if(r==1){
        uint8_t e1=((uint8_t*)src)[len-1];
        dest[j++]=base_map[e1>>2];
        dest[j++]=base_map[(e1<<4)&0x30];
    }else if(r==2){
        uint8_t e1=((uint8_t*)src)[len-2];
        uint8_t e2=((uint8_t*)src)[len-1];
        dest[j++]=base_map[e1>>2];
        dest[j++]=base_map[((e1<<4)|(e2>>4))&0x3F];
        dest[j++]=base_map[(e2<<2)&0x3C];
    }
    return j;
}

#define LEGAL_CHAR(c) (((c)>='A'&&(c)<='Z')||((c)>='a'&&(c)<='z')||((c)>='0'&&(c)<='9')||((c)=='-')||((c)=='_'))

static inline uint8_t base_map_r(char c){
    if(c>='A'&&c<='Z')
        return c-'A';
    else if(c>='a'&&c<='z')
        return c-'a'+26;
    else if(c>='0'&&c<='9')
        return c-'0'+52;
    else if(c=='-')
        return 62;
    else if(c=='_')
        return 63;
    else return 64;
}

size_t url_base64_decode(const char* src,size_t len,void* dest){
    if(len%4==1){
        return -1;
    }
    size_t i;
    for(i=0;i<len;++i){
        if(!LEGAL_CHAR(src[i])){
            return -1;
        }
    }
    size_t l=len/4;
    size_t r=len-l*4;
    size_t j=0;
    struct cell {char c1,c2,c3,c4;} c;
    uint8_t e1,e2,e3,e4;
    for(i=0;i<l;++i){
        c=((struct cell*)src)[i];
        e1=base_map_r(c.c1);
        e2=base_map_r(c.c2);
        e3=base_map_r(c.c3);
        e4=base_map_r(c.c4);
        ((uint8_t*)dest)[j++]=(e1<<2)|(e2>>4);
        ((uint8_t*)dest)[j++]=(e2<<4)|(e3>>2);
        ((uint8_t*)dest)[j++]=(e3<<6)|e4;
    }
    if(r==2){
        e1=base_map_r(src[len-2]);
        e2=base_map_r(src[len-1]);
        ((uint8_t*)dest)[j++]=(e1<<2)|(e2>>4);
    }else if(r==3){
        e1=base_map_r(src[len-3]);
        e2=base_map_r(src[len-2]);
        e3=base_map_r(src[len-1]);
        ((uint8_t*)dest)[j++]=(e1<<2)|(e2>>4);
        ((uint8_t*)dest)[j++]=(e2<<4)|(e3>>2);
    }
    return j;
}
