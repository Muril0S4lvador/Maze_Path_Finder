#include "deque.h"

struct Deque{
    data_type **data;
    int first_block;
    int first_array;
    int last_block;
    int last_array;
    int allocated_block;
};

struct Cel{
    int x;
    int y;
};

Cel *cel_construct(int x, int y){
    Cel *c = malloc(sizeof(Cel));
    c->x = x;
    c->y = y;
    return c;
}
void cel_print(void *v){
    Cel *c = (Cel*) v;
    printf("%d %d\n", c->x, c->y);
}
void cel_destroy(Cel *c){
    free(c);
}

Deque *deque_construct(){
    Deque *d = calloc(1, sizeof(Deque));

    d->data = calloc(BLOCKS_INITIAL_SIZE, sizeof(data_type*));
    d->allocated_block = BLOCKS_INITIAL_SIZE;
    d->first_block = d->last_block = BLOCKS_INITIAL_SIZE/2;
    d->first_array = d->last_array = 0;

    return d;
}

void deque_push_back(Deque *d, void *val){

    if(d->last_block >= d->allocated_block){
        if(d->first_block == 0){
            
            deque_data_realloc(d);
            deque_push_back(d, val);
            return;

        } else{

            /* Quando o fundo do deque se enche, o deque muda uma posicao para cima */
            data_type **new_data = (data_type**) calloc(d->allocated_block, sizeof(data_type*));
            int old_first = d->first_block, new_first = old_first - 1;

            while(old_first <= d->allocated_block - 1){

                new_data[new_first] = d->data[old_first];
                old_first++;
                new_first++;
            }
            d->last_block--;
            d->first_block--;
            d->first_array = 0;

            free(d->data);

            d->data = new_data;
            deque_push_back(d, val);
            return;
        }
    }
    
    if(d->last_array == 0)
        d->data[d->last_block] = calloc(ARRAY_SIZE, sizeof(data_type));

    d->data[d->last_block][d->last_array] = val;
    d->last_array++;

    if(d->last_array >= ARRAY_SIZE){
        d->last_array = 0;
        d->last_block++;
    }
    // printf("%d %d\n", d->last_block, d->last_array);
}

void deque_push_front(Deque *d, void *val){
    d->first_array--;

    if(d->first_array < 0){
        d->first_array = ARRAY_SIZE - 1;        
        d->first_block--;

    }

    if(d->first_block < 0){
        if( d->last_block == d->allocated_block || (d->last_block == d->allocated_block - 1 && d->last_array > 0) ){

            deque_data_realloc(d);
            d->first_block--;
            d->first_array = ARRAY_SIZE - 1;

        } else{
            /* Quando o topo do deque se enche, o deque muda uma posicao para baixo */
            data_type **new_data = (data_type**) calloc(d->allocated_block, sizeof(data_type*));
            int old_last = d->last_block, new_last = old_last + 1;

            while(old_last >= 0){

                if(d->data[old_last])
                    new_data[new_last] = d->data[old_last];
                old_last--;
                new_last--;
            }
            d->last_block++;
            d->first_block++;
            d->first_array = ARRAY_SIZE;

            if(d->data) free(d->data);

            d->data = new_data;
            deque_push_front(d, val);
            return;
        }
            
    }
    if(d->first_array == ARRAY_SIZE - 1)
        d->data[d->first_block] = calloc(ARRAY_SIZE, sizeof(data_type));

    d->data[d->first_block][d->first_array] = val;
}

void deque_data_realloc(Deque *d){

    if(d->first_block < 0) d->first_block = 0;

    int new_block, old_block = d->first_block, jump, old_size = d->allocated_block;

    d->allocated_block *= 2;
    jump = (d->allocated_block / 2) - (old_size / 2);

    data_type **new_data = calloc(d->allocated_block, sizeof(data_type*));

    while(old_block < old_size){
        new_block = old_block + jump;
        
        new_data[new_block] = d->data[old_block];
        old_block++;
    }
    d->first_block += jump;
    d->last_block += jump;

    if(d->data) free(d->data);

    d->data = new_data;
}

void *deque_pop_back(Deque *d){
    d->last_array--;

    if(d->last_array < 0){
        d->last_array = ARRAY_SIZE - 1;
        d->last_block--;
    }

    if(d->last_block < 0){
        d->last_block = 0;
        d->last_array = 0;

        return NULL;
    }

    void *v = d->data[d->last_block][d->last_array];

    if(d->last_array == 0)
        free(d->data[d->last_block]);

    return v;
}

void *deque_pop_front(Deque *d){
    void* v = d->data[d->first_block][d->first_array];

    d->first_array++;

    if(d->first_array >= ARRAY_SIZE){
        d->first_array = 0;
        free(d->data[d->first_block]);
        d->first_block++;
    }

    if(d->first_block >= d->allocated_block){
        d->first_block = d->last_block = d->allocated_block / 2;
    }
    
    return v;
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

void deque_destroy(Deque *d){
    void *v;

    while(deque_size(d)){
        if(d->first_block == d->last_block)
            if(d->first_array == d->last_array)
                break;

        v = deque_pop_front(d);
    }

    if(d->last_array)
        free(d->data[d->first_block]);
    free(d->data);
    free(d);
}

void printa_Coords(Deque *d){
    printf("\n<--- X --->\n");

    printf("First (B | A) -> %d %d\n", d->first_block, d->first_array);
    printf("Last (B | A) -> %d %d\n", d->last_block, d->last_array);
    printf("Allocated -> %d", d->allocated_block);

    printf("\n<--- X --->\n");
}
