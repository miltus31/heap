#include<math.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#include"heap.h"


static struct heap_node * build_full_binary_tree_rec(void *data, int nmemb, 
						size_t size, int *cur_dataindex,
						int level, int lastlevel,
						int *numremainlastlevel)
{
  struct heap_node *node;

  assert(*cur_dataindex < nmemb);
  /* dont branch if 
     1. level is lastlevel
     2. level is lastlevel - 1 and remainlastlevel is 0
     or 
     if level is lastlevel and remainlastlevel is 0 return null(easier)
  */

  if(level == lastlevel){
    if(*numremainlastlevel == 0)
      return NULL;
    else
      (*numremainlastlevel)--;
  }
  
  node = (struct heap_node *)malloc(sizeof(struct heap_node));

  //node->data = data + (*cur_dataindex)*size;
  // better to copy the data locally
  memcpy(node->data, data + (*cur_dataindex)*size, size);

  (*cur_dataindex)++;

  node->left = build_full_binary_tree_rec(data, nmemb, size, cur_dataindex,
					  level+1, lastlevel,
					  numremainlastlevel);

  node->right = build_full_binary_tree_rec(data, nmemb, size, cur_dataindex,
					   level+1, lastlevel,
					   numremainlastlevel);
  node->parent = NULL;

  if(node->left != NULL)
    node->left->parent = node;
  
  if(node->right != NULL)
    node->right->parent = node;

  return node;
}


/* will return a pointer to root node */
static struct heap_node * build_full_binary_tree(void *data, int nmemb, size_t size){
  int index = 0;

  //log w/o ceil gives the last level number starting from 0
  int lastlevelnum = (int) (log2f(nmemb)); 

  /* # of elements in the last level = 
     total elements - (total number of elements till the last-1 level)
     lastlevelnum is total levels - 1
  */
  int numremainlastlevel = nmemb - (1 << lastlevelnum) - 1;

  // each of the full level will have 2^i nodes
  /* this can be made implicit
     just keep track of current level and the level where to mind the number of 
     elements */
  
  return build_full_binary_tree_rec(data, nmemb, size, &index, 0, lastlevelnum, 
				    &numremainlastlevel);
}


/* this assumes node->left and node->right are valid min heaps
   the node may be violating the heap property and need to be pushed down
*/
static void heapify(heap *h, struct heap_node *node){
  struct heap_node *exchng;

  if(node->left != NULL){
    int cmp = h->compare(node, node->left);
    
    exchng = (cmp > 0)?node->left:node;
  
    if(node->right != NULL){
      cmp = h->compare(exchng, node->right);
      
      exchng = (cmp > 0)?node->right:exchng;
    }

    if(exchng != node){
      void *data = node->data;
      node->data = exchng->data;
      exchng->data = data;

      heapify(h, exchng);
    }
  }
  // the tree is full binary tree so no left child means no right child
}


static void validate_heap(heap *h, struct heap_node *node){
  if(node->left != NULL)
    validate_heap(h, node->left);

  if(node->right != NULL)
    validate_heap(h, node->right);

  /* heapify(h, node->left); */
  /* heapify(h, node->right); */
  heapify(h, node);
}


static struct heap_node *find_last_node(struct heap_node *node, int level,
					int lastlevelnum, int *remainlastlevel)
{
  struct heap_node *ret;

  if(level == lastlevelnum){
    if(*remainlastlevel == 1)
      return node;
    else
      (*remainlastlevel)--;
  }
  
  if(node->left != NULL){
    ret = find_last_node(node->left, level+1, lastlevelnum, remainlastlevel);

    if(ret != NULL)
      return ret;
  }

  if(node->right != NULL)
    return find_last_node(node->right, level+1, lastlevelnum, remainlastlevel);

  return NULL;
}


static void free_heap_r(struct heap_node *node){
  if(node->left != NULL)
    free_heap_r(node->left);

  if(node->right != NULL)
    free_heap_r(node->right);

  free(node->data);
  free(node);
}


heap * build_heap(void *data, int nmemb, size_t size
		  , int (*compare)(const void*, const void *) )
{
  //will not be same as done for array
  //create one tree(full binary tree)
  //go recursively in post order when returning
  //call heapify at the node

  heap *h = (heap *)malloc(sizeof(heap));

  h->root = build_full_binary_tree(data, nmemb, size);
  h->numnode = nmemb;
  h->compare = compare;

  validate_heap(h, h->root);

  return h;
}


void* extract_min(heap *h){
  /*find the last node
    exchange data with root
    remove its link from its parent
    free that node
  */
  void *return_data = h->root->data;

  //bfs would do but queue length can be large
  //do the same thing done in build_full_binary_tree, counting level and nodes
  // to get the last node
  int index = 0;
  int lastlevelnum = (int) log2f(h->numnode);

  int numlastlevel = h->numnode - (1 << lastlevelnum) - 1;

  struct heap_node *last_node = find_last_node(h->root, 0, lastlevelnum, 
					  &numlastlevel);

  h->root->data = last_node->data;
  if(last_node->parent->left = last_node)
    last_node->parent->left = NULL;
  else
    last_node->parent->right = NULL;

  free(last_node->data);
  free(last_node);

  return return_data;
}


void free_heap(heap *h){
  free_heap_r(h->root);

  free(h);
}
