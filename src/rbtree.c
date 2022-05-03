#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = nilNode;
  // p->root->parent = nilNode;
  return p;
};

void delete_node(rbtree *t, node_t *x) {
  if (x == t->nil) {
    return;
  }
  delete_node(t, x->left);
  delete_node(t, x->right);
  free(x);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

node_t *left_rotation(rbtree *t, node_t *x){
  node_t *y = x->right; 
  x->right = y->left; 

  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  x->parent = y;
  y->left = x;
  return t->root;
}

node_t *right_rotation(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  
  if (y->right != t->nil) { 
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  x->parent = y;
  y->right = x;
  return t->root;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  while (z->parent->color == RBTREE_RED) {
    // 경우 1, 2, 3
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      // 경우 1
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        // 경우 2
        if (z == z->parent->right) {
          z = z->parent;
          left_rotation(t, z);
        }
        // 경우 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotation(t, z->parent->parent);
      }
    // 경우 4, 5, 6  
    } else {
      y = z->parent->parent->left;
      // 경우 4
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else  {
        // 경우 5
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        // 경우 6
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
};

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));

  while (x != t->nil) {
    y = x;
    if (key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;
  if (y == t->nil) {
    t->root = z;
  } else if (key < y->key) {
    y->left = z;
  } else { 
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->key = key;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z;
};

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *ptr = t->root;
  while (ptr != t->nil) {
    if (key < ptr->key) {
      ptr = ptr->left;
    } else if (key > ptr->key) {
      ptr = ptr->right;
    } else {
      return ptr;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while (x->right != t->nil){
    x = x->right;
  }
  return x;
}

void rbtree_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK) {
    // 경우 1, 2, 3, 4
    if (x == x->parent->left) {
      w = x->parent->right;
      // 경우 1
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }
      // 경우 2
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        // 경우 3
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        // 경우 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }
    }
    // 경우 5, 6, 7, 8
    else {
      w = x->parent->left;
      // 경우 5
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }
      // 경우 6
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        // 경우 7
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        // 경우 8
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t *tree_successor(rbtree *t, node_t *p) {
  node_t *node = p->right;
  while (node->left != t->nil) {
    node = node->left;
  }
  return node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x, *y;

  // p의 nil 노드 체크
  if (p->left == t->nil || p->right == t->nil) {
    y = p;
  } else {
    y = tree_successor(t, p);
  }

  if (y->left != t->nil) {
    x = y->left;
  } else {
    x = y->right;
  }

  // 삭제후 연결작업
  x->parent = y->parent;
  if (y->parent == t->nil) {
    t->root = x;
  } else if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }

  if (y != p) {
    p->key = y->key;
  }

  if (y->color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }
  free(y);
  return 0;
}

int array_inorder(const rbtree *t, key_t *arr, const node_t *x, int i) {
  if (x->left != t->nil) {
    i = array_inorder(t, arr, x->left, i);
  }
  arr[i] = x->key;
  i++;
  if (x->right != t->nil) {
    i = array_inorder(t, arr, x->right, i);
  }
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  array_inorder(t, arr, t->root, 0);
  return 0;
}

void inorder(node_t* root, rbtree *t) {
  if (root == t->nil) {
    return;
  }
  inorder(root->left, t);
  printf("%d (%d) ", root->key, root->color);
  inorder(root->right, t);
}

// void main() {
//   rbtree *r1 = (rbtree*)calloc(1, sizeof(rbtree));
//   node_t *temp = (node_t*)calloc(1, sizeof(node_t));
//   node_t *minmax = (node_t*)calloc(1, sizeof(node_t));
//   r1 = new_rbtree();
//   rbtree_insert(r1, 1024);
//   rbtree_insert(r1, 1023);
//   rbtree_insert(r1, 1022);
//   rbtree_insert(r1, 10);
//   rbtree_insert(r1, 1025);
//   rbtree_insert(r1, 102);
//   rbtree_insert(r1, 109);
//   rbtree_insert(r1, 107);
//   rbtree_insert(r1, 107);
//   rbtree_insert(r1, 107);
//   rbtree_insert(r1, 108);
//   temp = rbtree_find(r1, 109);
//   printf("find : %d\n", temp->key);
//   minmax = rbtree_max(r1);
//   printf("max : %d\n", minmax->key);
//   minmax = rbtree_min(r1);
//   printf("min : %d\n", minmax->key);
  
//   inorder(r1->root, r1);
//   puts("");
//   rbtree_erase(r1, temp);
//   inorder(r1->root, r1);
//   puts("");
// }
