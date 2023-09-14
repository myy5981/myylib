# myylibc —— 我的C语言工具库

闲来没事造造轮子，一些功能与其调库不如自己写，看别人文档代码简直是究极折磨

写的时候优先考虑x86_64＋Linux＋GCC环境，仅支持使用gcc编译工具集编译。

## 先列一下flag，慢慢完成
真的很慢，也许只有游戏打累了才会写两行吧

### 加密模块
||项目|完成标记|更新时间|
|---:|:---|:---:|:---:|
|1|hex编码|☑|2023-1-12
|2|url_base64编码|☑|2023-1-12
|3|SM3密码杂凑与基于其的消息验证码和密钥导出函数|☑|2023-2-13
|4|SM4分组密码及其CBC模式实现|☑|2023-2-15
|5|SM4的GCM模式实现|□|
|6|ZUC序列密码|□|
|7|固定位数大数有限域运算|☑|2023-7-26
|8|SM2之公钥加密|☑|2023-7-26
|9|SM2之数字签名|☑|2023-8-19
|10|SM2之密钥交换|☑|2023-8-31
|11|SM9标识密码|□|

### 数据结构模块
|序号|项目|标记|时间|
|---:|:---|:---:|:---:|
|1|不可扩容的AVL树|□|
|2|不可扩容的HashMap|□|
|3|不可扩容的二叉堆与优先队列|□|

### 算法模块
|序号|项目|标记|时间|
|---:|:---|:---:|:---:|
|1|辗转相除法|☑|2023-5-10
|2|扩展欧几里得算法|☑|2023-5-10
|3|KMP字符串匹配|□|
|4|随机数生成|☑|2023-8-31
