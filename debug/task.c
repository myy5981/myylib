#include <myy/rb_tree.h>
#include <myy/random.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "task.h"

typedef struct _XXX_CTX {
	RB_NODE rbn;
	uint64_t key;
	uint64_t value;
}XXX_CTX;

typedef RB_TREE XXX_CTX_TREE;

int legal_rbtree(RB_NODE* root){
	if(root==NULL){
		return 0;
	}
	if(root->color==RED){
		if(root->left!=NULL&&root->left->color==RED){
			perror("illegal!\n");
			exit(1);
		}else if(root->right!=NULL&&root->right->color==RED){
			perror("illegal!\n");
			exit(1);
		}
	}
	int lh = legal_rbtree(root->left);
	int rh = legal_rbtree(root->right);
	if(lh!=rh){
		perror("illegal!\n");
		exit(1);
	}
	if(root->color==BLACK){
		lh++;
	}
	return lh;
}

int ctx_search(XXX_CTX_TREE* t, uint64_t k, uint64_t* v){
	XXX_CTX* x = (XXX_CTX*)t->root;
	while(x!=NULL){
		if(k<x->key){
			x=(XXX_CTX*)x->rbn.left;
		}else if(k>x->key){
			x=(XXX_CTX*)x->rbn.right;
		}else{
			*v=x->value;
			return 0;
		}
	}
	return 1;
}

int ctx_insert(XXX_CTX_TREE* t, XXX_CTX* ctx){
	ctx->rbn.left=NULL;
	ctx->rbn.right=NULL;
	ctx->rbn.p=NULL;
	RB_NODE* n = &(ctx->rbn);
	RB_NODE* x = t->root;
	if(x==NULL){
		t->root=n;
	}else{
		while(1){
			if(ctx->key < ((XXX_CTX*)x)->key){
				if(x->left==NULL){
					n->p=x;
					x->left=n;
					break;
				}
				x=x->left;
			}else if(ctx->key > ((XXX_CTX*)x)->key){
				if(x->right==NULL){
					n->p=x;
					x->right=n;
					break;
				}
				x=x->right;
			}else{
				((XXX_CTX*)x)->value=ctx->value;
				return 1;
			}
		}
	}
	rb_insert_fixup(t,(RB_NODE*)ctx);
	return 0;
}
#define ctx_remove(t,ctx) rb_remove_node((RB_TREE*)(t), (RB_NODE*)(ctx))

#define SIZE 2000000
XXX_CTX arr[SIZE];
int __index[SIZE]={0};
XXX_CTX_TREE t={.root=NULL};

int before(){
	for (int i = 0; i < SIZE; i++)
	{
		arr[i].key=i;
		arr[i].value=i;
		__index[i]=i;
	}
	uint32_t r;
	uint64_t temp;int te;
	for (int i = SIZE-1; i >= 0; i--)
	{
		rand_bytes((uint8_t*)&r,4);
		r=r%(i+1);
		temp=arr[i].key;
		arr[i].key=arr[r].key;
		arr[i].value=arr[r].value;
		arr[r].key=temp;
		arr[r].value=temp;

		rand_bytes((uint8_t*)&r,4);
		r=r%(i+1);
		te=__index[i];
		__index[i]=__index[r];
		__index[r]=te;
	}
	return 1;
}

void run(){
	for (int i = 0; i < SIZE; i++)
	{
		ctx_insert(&t,&arr[i]);
		//legal_rbtree(t.root);
	}
	
}

void search(){
	for (size_t i = 0; i < SIZE; i++)
	{
		uint64_t value;
		ctx_search(&t,__index[i],&value);
		//printf("r = %d, key = %d, value = %ld\n", r, __index[i], value);
	}
	
}

void delete(){
	for (int i = 0; i < SIZE; i++)
	{
		//printf("remove %ld\n",arr[__index[i]].key);
		ctx_remove(&t,&(arr[__index[i]]));
		//legal_rbtree(t.root);
	}
}

const Task tasks[]={
	{.before=before,.task=run,.after=NULL},
	{.before=NULL,.task=search,.after=NULL},
	{.before=NULL,.task=delete,.after=NULL}
};
int tasks_len=3;
