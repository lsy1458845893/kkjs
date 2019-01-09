#ifndef KKTREE_H
#define KKTREE_H

#include "kkdep.h"

// avl
typedef struct kkavl_node_s kkavl_node_t;

struct kkavl_node_s {
  kkavl_node_t *left;
  kkavl_node_t *right;
  kkuint8_t height;
};

static inline void kkavl_init(kkavl_node_t *node) {
  node->height = 1;
  node->left = 0;
  node->right = 0;
}
kkavl_node_t *kkavl_balance(kkavl_node_t *root);
kkavl_node_t *kkavl_remove(kkavl_node_t *root);

// splay
typedef struct kksplay_node_s kksplay_node_t;

struct kksplay_node_s {
  kksplay_node_t *left;
  kksplay_node_t *right;
};

static inline void kksplay_init(kksplay_node_t *node) {
  node->left = 0;
  node->right = 0;
}

kksplay_node_t *kksplay_left(kksplay_node_t *root);
kksplay_node_t *kksplay_right(kksplay_node_t *root);
kksplay_node_t *kksplay_remove(kksplay_node_t *root);

#endif
