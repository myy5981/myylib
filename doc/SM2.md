# 256位大数运算
代码文件：

> bignum.h  
> bignum.c  
> x86/bignum.h  
> x86/bignum.c

该部分主要是为了后面的GF(P)有限域运算服务的，并不能算一个功能完备的大数运算库。

## 类型定义

```c
typedef uint64_t BN_256[4];
```
在编译64位库时，使用4个64位整数以大端形式存储256位整数，512位同理。

## API

#### bn_256_add，bn_256_inc，bn_256_adc，bn_256_inc1

整数相加，add即r=a+b，无返回值，inc即r=r+b（这个函数好像没有必要写），adc则是返回进位的加法，因为有时候需要判断一个加法是否溢出，inc1是自增1。

该功能主要借助指令adcq实现

#### bn_256_sub

整数相减，减法不需要那么多功能，因为在后面的应用中会先判断大小再相减，该函数无返回值（即不会告知CF标志位状态）。

减法主要借助指令sbbq

#### bn_256_mul，bn_256_imul

两个256位数字相乘得到一个512位整数，将一个256位整数看成4位2^64进制数，借助mulq和adcq指令实现。

imul则是两个256位数相乘结果将其截断，保留后256位，该函数指令数量比mul少一大半，所以单独写了一个。后面蒙哥马利算法中会使用到这个功能。

#### bn_256_cmp

比较大小，```bn_256_cmp(a,b)>0```等价于```a>b```

#### bn_256_cpy,bn_256_is_zero,bn_256_is_one,bn_256_set_word

见名知意

#### bn_512_h256,bn_512_l256

这两个宏是在512位整数中取高256位和低256位，相当于除以2^256或模2^256，这不会产生新内存，如果需要提取到单独的内存，可以与bn_256_cpy组合使用：```bn_256_cpy(h,bn_512_h256(a))```;

## x86

因为上面用到了adcq,sbbq,mulq等x64指令，所以不能直接在32位环境下编译，在32位环境中，256位整数使用8个uint32_t位整数保存，使用32位的指令集，这样指令的数量大概是64位时的4倍。

项目跨平台编译没怎么整，需要将x86文件夹的内容拷贝到上级目录，然后修改一下Makefile

# SM2的GF(P)有限域运算
代码文件：

> sm2_bn.c  
> sm2_bn.h

在模P有限域中计算，其中：

P=0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF

## 类型定义
```c
typedef BN_256 BN_256_GFp;
typedef BN_256 BN_256_GFp_Mont;
```

## API

#### bn_256_GFp_add,bn_256_GFp_sub,bn_256_GFp_neg

即加减取负，因为是在有限域运算所以不用考虑进位了，

#### bn_256_GFp_mul

即乘法，使用巴雷特约减实现

#### bn_256_GFp_exp

求幂次，使用快速幂算法，这个函数目前没有排上用场。

#### bn_256_GFp_inv

求逆元，由于P是素数，根据费马小定理即为求P-2次幂，这个地方我参考了一下gmssl的算法，比起快速幂乘法次数更少。

#### bn_256_GFp_xxx_mont

上述各API在蒙哥马利下的实现，对于加减和原来的一致，只是取了别名，对于乘法以及基于乘法的求幂，求逆则使用蒙哥马利模乘实现，蒙哥马利约减比巴雷特约减效率稍快。

要使用此接口，需要先将数转到蒙哥马利表示，不要在计算单次乘法时转到蒙哥马利域再计算模乘再转回来，需要大规模计算乘法时才使用该接口。

# SM2点运算
代码文件：
> sm2_point.c  
> sm2.h

这部分文档中已经给出了公式，直接套用就行，稍微难点的在于如何高效利用尽可能少的中间变量以及尽可能少的计算次数。

SM2点主要有仿射坐标和jacobian加重射影坐标表示（文档中还给出了一种但是不常用），从仿射专jacobian只需要拷贝xy并将z置为1即可，反之，需要求z的逆元z_inv后：X=x*z_inv^2,Y=y*z_inv^3。

#### 倍点sm2_jpoint_dbl

公式文档中已给出，值得注意的是由于参数A=P-3，所以不需要真的把A代入进行乘法。

#### 点加sm2_jpoint_add_point

这个奇怪的算法是将一个jacobian点加上一个仿射坐标下的点，之所以这么弄，一是因为仿射点可以看做z=1的jacobian点，避免乘法计算，二是这个函数是为了点乘服务的，点乘中就是将一个jacobian点加上一个仿射坐标下的点，直接这样写省去坐标转换的麻烦。

公式文档中已给出，具体实现参考了gmssl的点加

因为目前还没有实现基于SM2的数字签名，所以这个加法够用了，但是后续还需要实现两个jacobian点的加法。

#### 点乘sm2_point_mul

这是将一个仿射坐标下的点乘上一个256位整数，内部实现是将点转为jacobian坐标后使用累加法计算。

#### 点与字节串的转换

官网给出了3中转换方式分别是普通表示，压缩表示，混合表示。

目前只有普通表示实现了从字节串到点的转换，因为其他两个表示需要计算开方，还没有实现该功能

#### sm2_xxx_mont

同样为了使用mont加速，上述功能均有蒙哥马利域的扩展函数，使用该功能需要先将点转到蒙哥马利域表示，即将点的xyz分别转到蒙哥马利域，其中1的蒙哥马利域是2^256 mod P。

# SM2公钥加密

代码文件：

> sm2_key.c  
> sm2_enc.c  
> sm2.h

包括密钥生成，公钥验证，公钥加密，私钥解密

随机数生成没有仔细实现，暂时用标准库写了个256位随机数生成，不是密码安全的

这个功能的函数就都能见名知意了，不再赘述。emm反过来说，上面的那几个函数确实有点不太优雅。。。所以为啥写这个文档，防止我自己几个月后就看不懂了