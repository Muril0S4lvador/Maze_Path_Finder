
#ifndef _HEAP_H_
#define _HEAP_H_

#include "types.h"
#include "hash.h"

typedef struct Heap Heap;
typedef struct HeapItem HeapItem;

#define HEAP_INITIAL_SIZE 19

Heap *heap_construct(HashTable *ht);
void *heap_push(Heap *heap, void *data, double priority);
bool heap_empty(Heap *heap);
void *heap_max(Heap *heap);
double heap_max_priority(Heap *heap);
double heap_min_priority(Heap *heap);
void *heap_pop(Heap *heap);
void heap_destroy(Heap *heap);

void heapify(HeapItem **data, int idx, HashTable *h, int size);
void heap_swap(HeapItem **data, int i, int j, HashTable *h);

HeapItem *heapitem_create(void *data, double priority);

int heap_parent(int idx);
int heap_left_child(int idx);
int heap_right_child(int idx);

void printheap(Heap *heap);

#endif