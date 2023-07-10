#include <myy/sm2_bn.h>

const BN_256 SM2_P                   = BN_256_INIT(FFFFFFFE,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00000000,FFFFFFFF,FFFFFFFF);
// 2^256-p
static const BN_256 TWO_POWED_256_SUB_SM2_P = BN_256_INIT(00000001,00000000,00000000,00000000,00000000,FFFFFFFF,00000000,00000001);

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

void bn_256_GFp_mul(BN_256_GFp r, const BN_256_GFp a, const BN_256_GFp b){

}
