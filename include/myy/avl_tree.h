#ifndef MYY_AVL_TREE
#define MYY_AVL_TREE

#include <myy/env.h>

__CPP_BEGIN

struct _AVL_NODE {
    struct _AVL_NODE* p;
    struct _AVL_NODE* left;
    struct _AVL_NODE* right;
    int color;
};

typedef struct _AVL_NODE AVL_NODE;

typedef struct {
    struct _AVL_NODE* root;
} AVL_TREE;

/**
 * 在插入n后，对树进行平衡调整
 */
extern void avl_insert_fixup(AVL_TREE* t, AVL_NODE* n);

/**
 * 从t中移除n，必须保证n在t中，否则会导致段错误
 */
extern void avl_remove_node(AVL_TREE* t, AVL_NODE* n);

__CPP_END

#endif
