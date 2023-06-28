#include <stdlib.h>
#include "deque.h"
#include "vector.h"

struct Deque{
    data_type **data;
    int first_block;
    int last_block;
    int allocated_block;
    int first_array;
    int last_array;
};

Deque *deque_construct(){
    Deque *d = calloc(1, sizeof(Deque));

    d->data = calloc(BLOCKS_INITIAL_SIZE, sizeof(data_type*));
    d->allocated_block = BLOCKS_INITIAL_SIZE;
    d->first_block = d->last_block = BLOCKS_INITIAL_SIZE/2;
    d->first_array = d->last_array = 0;

    return d;
}

void deque_push_back(Deque *d, void *val){
    if(d->last_array == 0)
        d->data[d->last_block] = calloc(ARRAY_SIZE, sizeof(data_type));

    d->data[d->last_block][d->last_array] = val;
    d->last_array++;

    if(d->last_array >= ARRAY_SIZE){
        d->last_array = 0;
        d->last_block++;
    }

    if(d->last_block >= d->allocated_block){
        //realoca
        
    }
}

void deque_push_front(Deque *d, void *val){
    d->first_array--;

    if(d->first_array < 0){
        d->first_array = ARRAY_SIZE - 1;        
        d->first_block--;
    }

    if(d->first_block < 0){
        // realoca
    }

    d->data[d->first_block][d->first_array] = val;
}

void *deque_pop_back(Deque *d){
    d->last_array--;

    if(d->last_array < 0){
        d->last_array = ARRAY_SIZE - 1;
        free(d->data[d->last_block]);
        d->last_block--;
    }

    if(d->last_block < 0){
        d->last_block = 0;
        d->last_array = 0;

        return NULL;
    }

    void* v = d->data[d->last_block][d->last_array];
    free(d->data[d->last_block][d->last_array]);

    return v;
}

void *deque_pop_front(Deque *d){
    void* v = d->data[d->first_block][d->first_array];
    free(d->data[d->first_block][d->first_array]);

    d->first_array++;

    if(d->first_array >= ARRAY_SIZE){
        d->first_array = 0;
        d->first_block++;
    }

    if(d->first_block >= d->allocated_block){
        d->first_block = d->allocated_block - 1;
        return NULL;
    }
}





int deque_size(Deque *d){
    return (d->last_block - d->first_block) *
    ARRAY_SIZE + (d->first_array - d->last_array);
}

void *deque_get(Deque *d, int idx){
    int pos = d->first_array + idx;
    int bloco_idx = pos / ARRAY_SIZE;
    int item_idx = pos % ARRAY_SIZE;
}
