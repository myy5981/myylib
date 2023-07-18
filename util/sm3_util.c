#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <myy/sm3.h>
#include <myy/hex.h>

#define __SM3_UTIL_HELP__ "usage: sm3 <file_path> ...\n"

static uint8_t buf[1024]={0};
static SM3_CTX ctx;
static uint8_t hash[SM3_DIGEST_LEN]={0};
static char hex[SM3_DIGEST_LEN*2+1]={0};

void sm3_file(const char* file_path){
	FILE* f=fopen(file_path,"rb");
	if(f==NULL){
		fprintf(stderr,"fail to open file %s:%s\n",file_path,strerror(errno));
		return;
	}
	sm3_init(&ctx);
	size_t r=0;
	while ((r=fread(buf,sizeof(uint8_t),1024,f))>0)
	{
		if(sm3_update(&ctx,buf,r)){
			fprintf(stderr,"sm3: 数据过于庞大，超过SM3最大数据限制\n");
			return;
		}
	}
	if(r==0){
		sm3_final(&ctx,hash);
		hex_encode(hash,SM3_DIGEST_LEN,hex,HEX_ENCODE_LOWER);
		printf("%s (%s)\n",hex,file_path);
	}else if(r<0){
		fprintf(stderr,"fail to read from %s:%s\n",file_path,strerror(errno));
	}
	fclose(f);
}

int main(int argc,const char* argv[]){
	int i=argc-1;
	if(i<=0){
		printf(__SM3_UTIL_HELP__);
		return 0;
	}
	for(int j=0;j<i;++j){
		sm3_file(argv[j+1]);
	}
	return 0;
}
