#pragma once

#include "list.h"
#include "tree.h"
#include <stdlib.h>

typedef struct page {
  size_t order;
  size_t size;
  size_t *keys;
  void **vals;
  struct page **branches;
  struct page *top;
  int __leaf;
} * page_t;

/*
typedef struct page_list {
  size_t order;
  list_t vals;
  node_t root;
} * plist_t;
*/

page_t btree_new_page(size_t order);

/*
plist_t plist_new_list(size_t order);
*/

page_t btree_insert(size_t key, void *value, page_t page);
page_t btree_split(page_t page);

void btree_preorder(page_t page, int lvl);
void btree_inorder(page_t page, int lvl);

// vim:ft=c:ts=2:sw=2:sts=2:et
