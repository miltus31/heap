#ifndef HEAP_H
#define HEAP_H

/* Min heap Tree implementation*/

typedef struct{
  struct heap_node* root;

  int numnode;
  //  size_t size;

  //comparison function
  int (*compare)(const void *, const void *);
}heap;

struct heap_node{
  void *data;

  struct heap_node *left, *right, *parent;
};


heap * build_heap(void *data, int nmemb, size_t size
		 , int (*compare)(const void*, const void *) );

void * extract_min(heap *h);

void free_heap(heap *h);

#endif
