#include "tree/b.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

page_t btree_new_page(size_t order) {
  page_t page = (page_t)malloc(sizeof(struct page));
  assert(page);
  page->size = 0;
  page->order = order;
  page->keys = (size_t *)calloc(order, sizeof(size_t));
  page->vals = (void **)calloc(order, sizeof(void *));
  page->branches = NULL;
  page->top = NULL;
  page->__leaf = 0;
  return page;
}

/*
plist_t plist_new_list(size_t order) {
  plist_t list = (plist_t)malloc(sizeof(struct page_list));
  list->order = order;
  list->vals = L.new();
  page_t page = plist_new_page(order);
  list->root = T.new(page);
  return list;
}
*/

page_t btree_insert(size_t key, void *value, page_t page) {
  page_t it = page;
  while (it->branches) { // Não é uma folha
    size_t i;
    for (i = 0; i < it->size && key > it->keys[i]; i++)
      ;
    if (key == it->keys[i]) {
      it->vals[i] = value;
      return page;
    }
    it = it->branches[i];
  }

  size_t i;
  for (i = 0; i < it->size && key > it->keys[i]; i++)
    ;
  if (key == it->keys[i]) {
    it->vals[i] = value;
  } else {
    if (it->size) {
      memmove(it->keys + i + 1, it->keys + i, sizeof(size_t) * (it->size - i));
      memmove(it->vals + i + 1, it->vals + i, sizeof(void *) * (it->size - i));
    }
    it->keys[i] = key;
    it->vals[i] = value;
    it->size++;
  }

  if (it->size == it->order) { // tamanho máximo
    it = btree_split(it);
    if (!it->top) {
      return it;
    }
  }
  return page;
}

page_t btree_split(page_t page) {
  page_t ret = page;
  page_t left = btree_new_page(page->order),
         right = btree_new_page(page->order);
  left->top = right->top = page->top;
  size_t median_pos = page->size / 2 - !(page->size % 2);
  size_t median = page->keys[median_pos];
  for (size_t i = 0; i < page->size; i++) {
    if (page->keys[i] < median) {
      btree_insert(page->keys[i], page->vals[i], left);
    } else if (page->keys[i] > median) {
      btree_insert(page->keys[i], page->vals[i], right);
    }
  }
  if (page->top) {
    size_t i;
    for (i = 0; i < page->top->size && median > page->top->keys[i]; i++)
      ;
    if (page->top->size) {
      memmove(page->top->keys + i + 1, page->top->keys + i,
              sizeof(size_t) * (page->top->size - i));
      memmove(page->top->vals + i + 1, page->top->vals + i,
              sizeof(void *) * (page->top->size - i));
      memmove(page->top->branches + i + 1, page->top->branches + i,
              sizeof(page_t) * (page->top->size - i));
    }
    page->top->keys[i] = median;
    page->top->vals[i] = page->vals[median_pos];
    page->top->branches[i] = left;
    page->top->branches[i + 1] = right;
    page->top->size++;
    if (page->top->size == page->top->order) {
      ret = btree_split(page->top);
    }
  } else {
    page_t top = btree_new_page(page->order);
    top->branches = (page_t *)calloc(top->order + 1, sizeof(page_t));
    top->keys[0] = median;
    top->vals[0] = page->vals[median_pos];
    top->branches[0] = left;
    top->branches[1] = right;
    top->size++;
    ret = left->top = right->top = top;
  }
  free(page->keys);
  free(page->vals);
  free(page->branches);
  free(page);
  return ret;
}

void btree_preorder(page_t page, int lvl) {
  printf("lvl: %d\n", lvl);
  for (int i = 0; i < page->size; i++) {
    printf("\t%lu\n", page->keys[i]);
  }
  if (page->branches)
    for (int i = 0; i < page->size + 1; i++)
      btree_preorder(page->branches[i], lvl + 1);
}

void btree_inorder(page_t page, int lvl) {
  if (page->branches) {
    for (int i = 0; i < page->size + 1; i++) {
      btree_inorder(page->branches[i], lvl + 1);
      if (i < page->size) {
        printf("\t%lu\n", page->keys[i]);
      }
    }
  } else {
    printf("lvl: %d\n", lvl);
    for (int i = 0; i < page->size; i++) {
      printf("\t%lu\n", page->keys[i]);
    }
  }
}

// vim:ft=c:ts=2:sw=2:sts=2:et
