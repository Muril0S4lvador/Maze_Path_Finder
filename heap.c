#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

struct HeapItem{
    void *data;
    double priority;
};

struct Heap{
    HeapItem **data;
    HashTable *ht;
    int size;
    int allocated;
};

HeapItem *heapitem_create(void *data, double priority){
    HeapItem *hi = malloc(sizeof(HeapItem));

    hi->data = data;
    hi->priority = priority;

    return hi;
}

int heap_parent(int idx){
    return (idx - 1) / 2;
}

int heap_left_child(int idx){
    return 2 * idx + 1;
}

int heap_right_child(int idx){
    return 2 * idx + 2;
}

bool heap_empty(Heap *heap){
    return !heap->size;
}

Heap *heap_construct(HashTable *ht){
    Heap *h = malloc(sizeof(Heap));

    h->data = calloc(HEAP_INITIAL_SIZE, sizeof(HeapItem*));
    h->ht = ht;
    h->size = 0;
    h->allocated = HEAP_INITIAL_SIZE;

    return h;
}

void *heap_push(Heap *heap, void *data, double priority){

    void *v = NULL;
    int *v2 = hash_table_get(heap->ht, data);
    int idx;

    if(v2){

        if (v2) idx = *v2;

        if(heap->data[idx]->priority > priority){
            heap->data[idx]->priority = priority;
            heapify(heap->data, idx, heap->ht, heap->size);
        }

        return data;

    } else {

        int *idx_ptr = malloc(sizeof(int)), size = heap->size;
        *idx_ptr = size;
        HeapItem *hi = heapitem_create(data, priority);
        heap->data[heap->size] = hi;
        v = hash_table_set(heap->ht, data, idx_ptr);

        heapify(heap->data, heap->size, heap->ht, heap->size);

        heap->size++;

        if(heap->size >= heap->allocated){
            heap->allocated *= 2;
            heap->data = realloc(heap->data, heap->allocated * sizeof(HeapItem*));
        }
    }
    return NULL;
}

void heapify(HeapItem **data, int idx, HashTable *h, int size){
    int pai = heap_parent(idx), rchild = heap_right_child(idx), lchild = heap_left_child(idx);

    if( idx != pai && data[idx]->priority < data[pai]->priority ){

       heap_swap(data, idx, pai, h);

        heapify(data, idx, h, size);
        heapify(data, pai, h, size);
        return;

    }
    
    if( lchild < size && data[idx]->priority > data[lchild]->priority ){
        
        if( rchild >= size || ( rchild < size && data[rchild]->priority >= data[lchild]->priority ) ){

            heap_swap(data, idx, lchild, h);

            heapify(data, idx, h, size);
            heapify(data, lchild, h, size);
            return;
        }
    }

    if( rchild < size && data[idx]->priority > data[rchild]->priority ){
        
       heap_swap(data, idx, rchild, h);

        heapify(data, idx, h, size);
        heapify(data, rchild, h, size);
        return;
    }
}

void heap_swap(HeapItem **data, int i, int j, HashTable *h){
    // Troca de lugar heap[i] com heap[j] e atualiza sua hash

    HeapItem *aux;

    aux = data[i];
    data[i] = data[j];
    data[j] = aux;

    int *ptr = malloc(sizeof(int)),
        *ptr2 = malloc(sizeof(int));
    void *v;

    *ptr = i;
    v = hash_table_set(h, data[i]->data, ptr);
    if(v) free(v);
    
    *ptr2 = j;
    v = hash_table_set(h, data[j]->data, ptr2);
    if(v) free(v);

}

double heap_min_priority(Heap *heap){
    return heap->data[0]->priority;
}

void *heap_pop(Heap *heap){

    HashTableItem *htp = hash_table_pop(heap->ht, heap->data[0]->data);
    void *v = htp->key;

    free(htp->val);
    free(htp);

    heap->size--;

    if(heap->size > 0){
        HeapItem *aux = heap->data[0];
        heap->data[0] = heap->data[heap->size];
        heap->data[heap->size] = NULL;
        free(aux);

        int *ptr = malloc(sizeof(int));
        *ptr = 0;
        void *x = hash_table_set(heap->ht, heap->data[0]->data, ptr);
        if(x) free(x);

        heapify(heap->data, 0, heap->ht, heap->size);

    } else if(heap->size == 0){

        free(heap->data[0]);
    }

    return v;
}

void heap_destroy(Heap *heap){
    for(int i = 0; i < heap->allocated; i++){
        if(heap->data[i]){
            // printf("heap->data[%d]\n", i);
            // free(heap->data[i]);
        }
    }
    free(heap->data);
    free(heap);
}

void printheap(Heap *heap){
    int vetor[heap->size];
    int ha = 0, *h = NULL;

    printf("\n[");
    for(int i = 0; i < heap->size; i++){
        printf("%0.lf", heap->data[i]->priority);
        if(i < heap->size - 1) printf(", ");

        // pegar idx no heap pela hash
        void *item = hash_table_get(heap->ht, heap->data[i]->data);
        if(item) h = (int *) item;
        if(h) ha = *h;
        vetor[i] = ha;
    }
    printf("]\n");

    printf("[");
    for(int i = 0; i < heap->size; i++){
        printf("%d", vetor[i]);
        if(i < heap->size - 1) printf(", ");
    }
    printf("]\n");

}