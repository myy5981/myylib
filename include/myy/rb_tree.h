#ifndef MYY_RBTREE_H
#define MYY_RBTREE_H

#include <myy/env.h>
#include <stdint.h>

#define RED 1
#define BLACK 0

__CPP_BEGIN

/**
 * 使用本API时，需将RB_NODE置于希望使用红黑树管理的结构体首部：
 * 	typedef struct _XXX_CTX{
 * 		RB_NODE rbn;
 * 		uint64_t key;
 * 		void* value;
 * 	}XXX_CTX;
 * 	typedef RB_TREE XXX_CTX_MAP;
 * 如此一来指向RB_NODE的指针同时也是指向结构体的指针，若不放在结构体头则不可直接类型转换
 * 
 * 需要自行添加插入逻辑，这取决于调用者需求，以下是可供参考的方式（与普通二叉搜索树相同）：
 * 
 * 	int ctx_insert(XXX_CTX_TREE* t, XXX_CTX* ctx){
 * 		ctx->rbn.left=NULL;
 * 		ctx->rbn.right=NULL;
 * 		ctx->rbn.p=NULL;
 * 		RB_NODE* n = &(ctx->rbn);
 * 		RB_NODE* x = t->root;
 * 		if(x==NULL){
 * 			t->root=n;
 * 		}else{
 * 			for(;;){
 * 				if(ctx->key < ((XXX_CTX*)x)->key){
 * 					if(x->left==NULL){
 * 						n->p=x;
 * 						x->left=n;
 * 						break;
 * 					}
 * 					x=x->left;
 * 				}else if(ctx->key > ((XXX_CTX*)x)->key){
 * 					if(x->right==NULL){
 * 						n->p=x;
 * 						x->right=n;
 * 						break;
 * 					}
 * 					x=x->right;
 * 				}else{
 * 					((XXX_CTX*)x)->value=ctx->value;
 * 					return 1;
 * 				}
 * 			}
 * 		}
 * 		rb_insert_fixup(t,(RB_NODE*)ctx);
 * 		return 0;
 * 	}
 * 	
 * 	int ctx_search(XXX_CTX_TREE* t, uint64_t k, uint64_t* v){
 * 		XXX_CTX* x = (XXX_CTX*)t->root;
 * 		while(x!=NULL){
 * 			if(k<x->key){
 * 				x=(XXX_CTX*)x->rbn.left;
 * 			}else if(k>x->key){
 * 				x=(XXX_CTX*)x->rbn.right;
 * 			}else{
 * 				*v=x->value;
 * 				return 0;
 * 			}
 * 		}
 * 		return 1;
 * 	}
 * 	
 * 	XXX_CTX* ctx_remove(XXX_CTX_TREE* t, uint64_t k){
 * 		XXX_CTX* x = (XXX_CTX*)t->root;
 * 		while(x!=NULL){
 * 			if(k<x->key){
 * 				x=(XXX_CTX*)x->rbn.left;
 * 			}else if(k>x->key){
 * 				x=(XXX_CTX*)x->rbn.right;
 * 			}else{
 * 				rb_remove_node(t,&(x->rbn));
 * 				return x;
 * 			}
 * 		}
 * 		return NULL;
 * 	}
 * 	
*/

struct _RB_NODE{
	struct _RB_NODE* p;
	struct _RB_NODE* left;
	struct _RB_NODE* right;
	int color;
};

typedef struct _RB_NODE RB_NODE;

typedef struct {
	struct _RB_NODE* root;
}RB_TREE;

/**
 * 在插入n后，对树进行平衡调整
*/
extern void rb_insert_fixup(RB_TREE* t, RB_NODE* n);

/**
 * 从t中移除n，必须保证n在t中，否则会导致段错误
*/
extern void rb_remove_node(RB_TREE* t, RB_NODE* n);

__CPP_END

#endif
