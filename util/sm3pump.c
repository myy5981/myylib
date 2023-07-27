#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <myy/sm3.h>
#include <myy/hex.h>
#include <myy/endian.h>

//sm3(secret||myylibc)=eaf410c7963e12546e294b3d73dfdbef5753e364b817495072e5fdd649ea6075
//sm3(secret||myylibc+padding+append)=4c7fdb4f31c1091cca087b69edf997dc39d69aa06773964ec118dbfa703d0741
//                                    4c7fdb4f31c1091cca087b69edf997dc39d69aa06773964ec118dbfa703d0741
int main(int argc,char* argv[]){
	if(argc!=3){
		printf("usage: sm2pump [payload file path] [append file path]\n");
		return 0;
	}
	char hash_hex[65]={0};
	uint8_t hash[32];
	printf("本程序用于针对伪SM3_HMAC算法的hash长度扩展攻击\n");
	printf("请输入：\n哈希值（十六进制编码）：");
	fgets(hash_hex,65,stdin);
	fflush(stdin);
	size_t i = hex_decode(hash_hex,64,hash,HEX_DECODE_STRICT);
	if(i!=32){
		printf("不是SM3散列%ld\n",i);
		return 0;
	}
	SM3_CTX _ctx,*ctx=&_ctx;
	sm3_init(ctx);

	ctx->V[0]=be2h_32(((uint32_t*)hash)[0]);
	ctx->V[1]=be2h_32(((uint32_t*)hash)[1]);
	ctx->V[2]=be2h_32(((uint32_t*)hash)[2]);
	ctx->V[3]=be2h_32(((uint32_t*)hash)[3]);
	ctx->V[4]=be2h_32(((uint32_t*)hash)[4]);
	ctx->V[5]=be2h_32(((uint32_t*)hash)[5]);
	ctx->V[6]=be2h_32(((uint32_t*)hash)[6]);
	ctx->V[7]=be2h_32(((uint32_t*)hash)[7]);

	printf("密钥长度：");
	int secret_len;
	scanf("%d",&secret_len);

	FILE* f = fopen(argv[1],"r+");
	if(f==NULL){
		printf("fail to open %s : %s\n",argv[1],strerror(errno));
		return 0;
	}
	fseek(f,0L,SEEK_END);
  	long payload_len=ftell(f);
	if(payload_len>=1024*1024*1024){
		printf("数据规模过大\n");
		return 0;
	}

	ctx->len=(size_t)payload_len+(size_t)secret_len;

	FILE* f1 = fopen(argv[2],"r");
	if(f1==NULL){
		printf("fail to open %s : %s\n",argv[2],strerror(errno));
		return 0;
	}

	// 写入填充字节
	size_t r=64-ctx->len%64;
	size_t bit_len=ctx->len*8;
	char buf[64]={0};
	*((uint64_t*)(buf+56))=h2be_64(bit_len);
	if(r==64){
		buf[0]='\x80';
		fwrite(buf,1,64,f);
		ctx->len=ctx->len+64;
	}else if(r<9){//作两块
		fwrite("\x80\0\0\0\0\0\0\0",1,r,f);
		fwrite(buf,1,64,f);
		ctx->len=(ctx->len/64l+2l)*64l;
	}else{//作一块
		buf[64-r]='\x80';
		fwrite(buf+64-r,1,r,f);
		ctx->len=(ctx->len/64l+1l)*64l;
	}

	// 计算新的散列
	while ((r=fread(buf,1,64,f1))>0)
	{
		sm3_update(ctx,buf,r);
		fwrite(buf,1,r,f);
	}
	if(r!=0){
		printf("fail to read payload : %s\n",strerror(errno));
		return 1;
	}
	sm3_final(ctx,hash);
	printf("新的哈希值：");
	hex_enc2stream(stdout,hash,32);
	printf("\n填充后的负载已写入至原始负载文件中，可自行选择编码方式打开\n");
	return 0;
}