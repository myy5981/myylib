#ifndef MYY_RBTREE_H
#define MYY_RBTREE_H

#include <stdint.h>

#define RED 1
#define BLACK 0

struct _RB_NODE{
	struct _RB_NODE* p;
	struct _RB_NODE* left;
	struct _RB_NODE* right;
	uint64_t key;
	uint32_t value;
	int color;
};

typedef struct _RB_NODE RB_NODE;

typedef struct {
	struct _RB_NODE* root;
}RB_TREE;

/**
 * 向t中插入n，若已存在相同的n->key，则会替换value，并返回1
 * 否则成功插入并返回0，
 * 注意：该函数直接使用传入n的内存空间，调用该函数后不可释放n
*/
int rb_insert_node(RB_TREE* t, RB_NODE* n);

/**
 * 从t中移除n，必须保证n在t中
*/
void rb_remove_node(RB_TREE* t, RB_NODE* n);

#endif
