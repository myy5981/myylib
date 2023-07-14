#include <myy/sm2_bn.h>

const BN_256 SM2_P                          = BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF);
static const BN_512 SM2_P_512               = {0,0,0,0,0,0,0,0,0xFFFFFFFEu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0xFFFFFFFFu,0x00000000u,0xFFFFFFFFu,0xFFFFFFFFu};

// 2^256-p
static const BN_256 TWO_POWED_256_SUB_SM2_P = BN_256_INIT(00000001,00000000,00000000,00000000,00000000,FFFFFFFF,00000000,00000001);

// 2^512/p-2^256
static const BN_256 TWO_POWED_512_DIV_SM2_P = BN_256_INIT(00000001,00000001,00000001,00000001,00000002,00000002,00000002,00000003);

void bn_256_GFp_add(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
    if(bn_256_add(r,a,b)!=0){
        __bn_256_add(r,r,TWO_POWED_256_SUB_SM2_P);
    }else if(bn_256_cmp(r,SM2_P)>=0){
        bn_256_sub(r,r,SM2_P);
    }
}

void bn_256_GFp_sub(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
    if(bn_256_cmp(a,b)>=0){
        bn_256_sub(r,a,b);
    }else{
        bn_256_sub(r,SM2_P,b);
        __bn_256_add(r,r,a);
    }
}

void bn_256_GFp_neg(BN_256_GFp r, const BN_256_GFp a){
    if(bn_256_is_zero(a)){
        if(r!=a){
            bn_256_cpy(r,a);
        }
    }else{
        bn_256_sub(r,SM2_P,a);
    }
}

void bn_256_GFp_half(BN_256_GFp r, const BN_256_GFp a){
    int cf=0;
    if(bn_256_is_odd(a)){
        cf=bn_256_add(r,a,SM2_P);
    }else{
        bn_256_cpy(r,a);
    }
    bn_256_right(r,1);
    //补上溢出的一位
    r[0]|=(cf<<(UNIT_LEN-1));
}

// Barrett Reduction
void bn_256_GFp_mul(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){
    BN_256 q1,q2;
    #define q3 q2
    BN_512 X,q3p,R;
    bn_256_mul(X,a,b);
    bn_512_hhalf(q1,X);
    bn_256_dmul(q2,q1,TWO_POWED_512_DIV_SM2_P);
    bn_256_add(q3,q2,q1);
    bn_256_mul(q3p,q3,SM2_P);
    bn_512_sub(R,X,q3p);
    while(bn_512_cmp(R,SM2_P_512)>0){
        bn_512_sub(R,R,SM2_P_512);
    }
    bn_512_lhalf(r,R);
    #undef q3
}
