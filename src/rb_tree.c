#include <myy/rb_tree.h>
#include <stddef.h>

static inline void left_rotate(RB_TREE* t,RB_NODE* x) {
	RB_NODE* y=x->right;
	x->right=y->left;
	if(y->left!=NULL){
		y->left->p=x;
	}
	y->p=x->p;
	if(x->p==NULL){
		t->root=y;
	}else if(x==x->p->left){
		x->p->left=y;
	}else{
		x->p->right=y;
	}
	y->left=x;
	x->p=y;
}

static inline void right_rotate(RB_TREE* t,RB_NODE* x) {
	RB_NODE* y=x->left;
	x->left=y->right;
	if(y->right!=NULL){
		y->right->p=x;
	}
	y->p=x->p;
	if(x->p==NULL){
		t->root=y;
	}else if(x==x->p->left){
		x->p->left=y;
	}else{
		x->p->right=y;
	}
	y->right=x;
	x->p=y;
}

void rb_insert_fixup(RB_TREE* t, RB_NODE* n) {
	n->color=RED;
	RB_NODE* x;
	while(n->p!=NULL&&n->p->color==RED){
		if(n->p==n->p->p->left){
			x=n->p->p->right;
			if(x!=NULL&&x->color==RED){
				n->p->color=BLACK;
				x->color=BLACK;
				n->p->p->color=RED;
				n=n->p->p;
			}else {
				if(n==n->p->right){
					n=n->p;
					left_rotate(t,n);
				}
				n->p->color=BLACK;
				n->p->p->color=RED;
				right_rotate(t,n->p->p);
			}
		}else{
			x=n->p->p->left;
			if(x!=NULL&&x->color==RED){
				n->p->color=BLACK;
				x->color=BLACK;
				n->p->p->color=RED;
				n=n->p->p;
			}else {
				if(n==n->p->left){
					n=n->p;
					right_rotate(t,n);
				}
				n->p->color=BLACK;
				n->p->p->color=RED;
				left_rotate(t,n->p->p);
			}
		}
	}
	t->root->color=BLACK;
	return;
}

static inline RB_NODE* rb_minimum(RB_NODE* root){
	while(root->left!=NULL){
		root=root->left;
	}
	return root;
}

void rb_remove_node(RB_TREE* t, RB_NODE* n){
	int c;
	RB_NODE* x;
	// 处理含两个子节点的情况，调换一下继续
	if(n->left!=NULL&&n->right!=NULL){
		x=rb_minimum(n->right);
		if(x==n->right){
			x->p=n->p;
			if(n->p!=NULL){
				if(n==n->p->left){
					n->p->left=x;
				}else{
					n->p->right=x;
				}
			}else{
				t->root=x;
			}
			n->p=x;
			x->left=n->left;
			n->left->p=x;
			n->left=NULL;
			n->right=x->right;
			if(x->right!=NULL){
				x->right->p=n;
			}
			x->right=n;
			c=x->color;
			x->color=n->color;
			n->color=c;
		}else{
			RB_NODE* xp=x->p;
			RB_NODE* xr=x->right;
			x->p=n->p;
			if(n->p!=NULL){
				if(n==n->p->left){
					n->p->left=x;
				}else{
					n->p->right=x;
				}
			}else{
				t->root=x;
			}
			n->p=xp;
			xp->left=n;
			x->right=n->right;
			n->right->p=x;
			x->left=n->left;
			n->left->p=x;
			n->left=NULL;
			n->right=xr;
			if(xr!=NULL){
				xr->p=n;
			}
			c=x->color;
			x->color=n->color;
			n->color=c;
		}
	}
	
	// 处理没有子结点的情况
	if(n->left==NULL&&n->right==NULL){
		if(n->color==RED){
			if(n==n->p->left){
				n->p->left=NULL;
			}else{
				n->p->right=NULL;
			}
		}else{
			if(n==t->root){
				t->root=NULL;
				return;
			}
			//处理删除无子结点的黑节点
			struct _RB_NODE** d;
			if(n==n->p->left){
				d=&(n->p->left);
			}else{
				d=&(n->p->right);
			}
			while(1){
				if(n==t->root){
					break;
				}
				if(n==n->p->left){
					if(n->p->right->color==RED){
						n->p->right->color=BLACK;
						n->p->color=RED;
						left_rotate(t,n->p);
					}
					x=n->p->right;
					if((x->left==NULL||x->left->color==BLACK)&&(x->right==NULL||x->right->color==BLACK)){
						x->color=RED;
						if(n->p->color==RED){
							n->p->color=BLACK;
							break;
						}else{
							n=n->p;
							continue;
						}
					}
					if(x->left!=NULL&&x->left->color==RED){
						x->color=RED;
						x->left->color=BLACK;
						right_rotate(t,x);
					}
					x=n->p->right;
					left_rotate(t,n->p);
					x->color=n->p->color;
					x->left->color=BLACK;
					x->right->color=BLACK;
					break;
				}else{
					if(n->p->left->color==RED){
						n->p->left->color=BLACK;
						n->p->color=RED;
						right_rotate(t,n->p);
					}
					x=n->p->left;
					if((x->right==NULL||x->right->color==BLACK)&&(x->left==NULL||x->left->color==BLACK)){
						x->color=RED;
						if(n->p->color==RED){
							n->p->color=BLACK;
							break;
						}else{
							n=n->p;
							continue;
						}
					}
					if(x->right!=NULL&&x->right->color==RED){
						x->color=RED;
						x->right->color=BLACK;
						left_rotate(t,x);
					}
					x=n->p->left;
					right_rotate(t,n->p);
					x->color=n->p->color;
					x->right->color=BLACK;
					x->left->color=BLACK;
					break;
				}
			}
			*d=NULL;
		}
		return;
	}
	
	// 处理有一个子结点的情况
	if(n->left==NULL){
		x=n->right;
	}else{
		x=n->left;
	}
	x->p=n->p;
	x->color=BLACK;
	if(n->p!=NULL){
		if(n==n->p->left){
			n->p->left=x;
		}else{
			n->p->right=x;
		}
	}else{
		t->root=x;
	}
}
