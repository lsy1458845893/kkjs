#include "kktree.h"

// avl
static inline kkuint8_t kkavl_height(kkavl_node_t *root) { return root ? root->height : 0; }

static inline kkuint8_t kkavl_max(kkuint8_t a, kkuint8_t b) { return a > b ? a : b; }

static inline void kkavl_recount_height(kkavl_node_t *root) {
  root->height = 1 + kkavl_max(kkavl_height(root->left), kkavl_height(root->right));
}

//        r           a         r->left
//       / \         / \        a->right
//      a   b       c   r
//     / \             / \       
//    c   d           d   b
static inline kkavl_node_t *kkavl_rr(kkavl_node_t *root) {
  kkavl_node_t *a = root->left;
  root->left = a->right;
  a->right = root;
  kkavl_recount_height(root);
  kkavl_recount_height(a);
  return a;
}

//        r           b         r->right
//       / \         / \        b->left
//      a   b       r   d
//         / \     / \                   
//        c   d   a   c
static inline kkavl_node_t *kkavl_ll(kkavl_node_t *root) {
  kkavl_node_t *b = root->right;
  root->right = b->left;
  b->left = root;
  kkavl_recount_height(root);
  kkavl_recount_height(b);
  return b;
}

//      r        r       c
//     / \      / \     / \  
//    a   b    c   b   a   r
//     \      /             \ 
//      c    a               b
static inline kkavl_node_t *kkavl_lr(kkavl_node_t *root) {
  root->left = kkavl_ll(root->left);
  return kkavl_rr(root);
}

//      r       r          c
//     / \     / \        / \  
//    a   b   a   c      r   b
//       /         \    /
//      c           b  a
static inline kkavl_node_t *kkavl_rl(kkavl_node_t *root) {
  root->right = kkavl_rr(root->right);
  return kkavl_ll(root);
}

kkavl_node_t *kkavl_balance(kkavl_node_t *root) {
  if (root) {
    kkuint8_t hl = kkavl_height(root->left);
    kkuint8_t hr = kkavl_height(root->right);
    if (hl + 1 < hr) {
      if (kkavl_height(root->right->left) > kkavl_height(root->right->right))
        return kkavl_rl(root);
      else
        return kkavl_ll(root);
    } else if (hl > hr + 1) {
      if (kkavl_height(root->left->left) > kkavl_height(root->left->right))
        return kkavl_rr(root);
      else
        return kkavl_lr(root);
    } else {
      kkavl_recount_height(root);
      return root;
    }
  } else
    return 0;
}

static kkavl_node_t *kkavl_remove_l(kkavl_node_t **trace, kkavl_node_t *root) {
  kkavl_node_t *node = *trace;
  if (node) {
    if (node->right) {
      kkavl_node_t *upgrade = kkavl_remove_l(&node->right, root);
      *trace = kkavl_balance(*trace);
      return upgrade;
    } else {
      *trace = node->left;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

static kkavl_node_t *kkavl_remove_r(kkavl_node_t **trace, kkavl_node_t *root) {
  kkavl_node_t *node = *trace;
  if (node) {
    if (node->left) {
      kkavl_node_t *upgrade = kkavl_remove_r(&node->left, root);
      *trace = kkavl_balance(*trace);
      return upgrade;
    } else {
      *trace = node->right;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

kkavl_node_t *kkavl_remove(kkavl_node_t *root) {
  if (root) {
    kkuint8_t hl = kkavl_height(root->left);
    kkuint8_t hr = kkavl_height(root->right);
    if (hl != 0 || hr != 0) {
      kkavl_node_t *temp;
      if (hl > hr) {
        temp = root->left;
        root = kkavl_remove_l(&temp, root);
        root->left = temp;
      } else {
        temp = root->right;
        root = kkavl_remove_r(&temp, root);
        root->right = temp;
      }
      return kkavl_balance(root);
    } else
      return 0;
  } else
    return 0;
}

// splay

kksplay_node_t *kksplay_left(kksplay_node_t *root) {
  kksplay_node_t *a = root->left;
  root->left = a->right;
  a->right = root;
  return a;
}

kksplay_node_t *kksplay_right(kksplay_node_t *root) {
  kksplay_node_t *b = root->right;
  root->right = b->left;
  b->left = root;
  return b;
}

static kksplay_node_t *kksplay_remove_l(kksplay_node_t **trace, kksplay_node_t *root) {
  kksplay_node_t *node = *trace;
  if (node) {
    if (node->right) {
      kksplay_node_t *upgrade = kksplay_remove_l(&node->right, root);
      return upgrade;
    } else {
      *trace = node->left;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

static kksplay_node_t *kksplay_remove_r(kksplay_node_t **trace, kksplay_node_t *root) {
  kksplay_node_t *node = *trace;
  if (node) {
    if (node->left) {
      kksplay_node_t *upgrade = kksplay_remove_r(&node->left, root);
      return upgrade;
    } else {
      *trace = node->right;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

kksplay_node_t *kksplay_remove(kksplay_node_t *root) {
  if (root) {
    kksplay_node_t *temp;
    if (root->right) {
      temp = root->right;
      root = kksplay_remove_r(&temp, root);
      root->right = temp;
    } else if (root->left) {
      temp = root->left;
      root = kksplay_remove_l(&temp, root);
      root->left = temp;
    } else
      return 0;
    return root;
  } else
    return 0;
}
