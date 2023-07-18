#ifndef MYY_HEX_H
#define MYY_HEX_H

#include <myy/env.h>

#include <stdio.h>

__CPP_BEGIN

/**
 * 使用大写编码
*/
#define	HEX_ENCODE_UPPER	0
/**
 * 使用小写编码
*/
#define	HEX_ENCODE_LOWER	1
/**
 * 默认模式解码
 * 格式错误返回-1，但是可能已经写入一部分数据到dest中
*/
#define	HEX_DECODE_DEFAULT	0
/**
 * 严格模式解码
 * 在解码开始前先检查一遍src的合法性，格式错误返回-1，且不会写入任何数据dest中
 * 否则返回写入的字节数（一般为len的一半）
*/
#define	HEX_DECODE_STRICT	1
/**
 * 宽容模式解码
 * 遇到非HEX字符则跳过，若有效的HEX字符不是偶数则忽略最后一个有效的HEX字符
 * 返回写入的字节数
*/
#define	HEX_DECODE_TOLERANT	2
/**
 * 编码字节数组，并写入到dest中，最多编码2^63个，成功返回0，否则返回-1
 * （但是话说回来，HEX编码而已，还能怎么失败，当flag不对时失败）
 * flag可选：HEX_ENCODE_UPPER|HEX_ENCODE_LOWER
 * 注意：以字节为单位处理，会忽略系统的大小端模式
 *		这意味着，例如，在大端模式存储的A系统中使用该函数编码一个int数组，而在小端模式存储的B系统中解码会得到一个错误的结果
*/
extern	int		hex_encode		(void* src, size_t size, char* const dest, int flag);
/**
 * 解码HEX字符串，并写入到dest中，最多解码2^64个
 * flag可选：HEX_DECODE_DEFAULT|HEX_DECODE_STRICT|HEX_DECODE_TOLERANT
*/
extern	size_t	hex_decode		(const char* src, size_t len, void* dest, int flag);

/**
 * 编码到文件流中
*/
extern	void	hex_enc2stream	(FILE* fp, void* src, size_t len);

__CPP_END

#endif
