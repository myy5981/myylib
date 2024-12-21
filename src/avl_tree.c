#include <myy/avl_tree.h>

#define R(n) do{\
                if()\
            }while(0)

void avl_insert_fixup(AVL_TREE* t, AVL_NODE* n) {
    n->color = 0;
    int flag;
    while (n->p != NULL) {
        if (n == n->p->left) {
            n->p->color += 1;
            if (n->p->color == 0) {
                break;
            } else if (n->p->color == 1) {
                n = n->p;
                flag = 0;
                continue;
            } else {         // 失衡
                if (flag) {  // LR情况

                } else {  // R情况
                    if(n->p->p==NULL){//n的祖父是根节点
                        t->root=n;
                    }else if(n->p==n->p->p->left){
                        n->p->p->left=n;
                    }
                }
            }
        } else {
            n->p->color -= 1;
            if (n->p->color == 0) {
                break;
            } else if (n->p->color == -1) {
                n = n->p;
                flag = 1;
                continue;
            } else {         // 失衡
                if (flag) {  // RL情况

                } else {  // L情况

                }
            }
        }
    }

    return;
}

void avl_remove_node(AVL_TREE* t, AVL_NODE* n) {
    return;
}
