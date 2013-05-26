/* Copyright 2013 miltus31

This file is a part of heap

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
