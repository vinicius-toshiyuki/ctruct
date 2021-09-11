#include "tree/b.h"
#include <stdio.h>

int main() {
  page_t page = btree_new_page(3);
  page = btree_insert(1, NULL, page);
  page = btree_insert(2, NULL, page);
  page = btree_insert(3, NULL, page);
  page = btree_insert(4, NULL, page);
  page = btree_insert(5, NULL, page);
  btree_preorder(page, 1);
  printf("\n==========\n");
  btree_inorder(page, 1);
  return 0;
}
