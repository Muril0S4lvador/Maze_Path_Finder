
#include <stdlib.h>
#include "hash.h"
#include "forward_list.h"

struct HashTable
{
    ForwardList **buckets;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    int table_size;
    int n_elements;
};


HashTableItem *_hash_pair_construct(void *key, void *val)
{
    HashTableItem *p = calloc(1, sizeof(HashTableItem));
    p->key = key;
    p->val = val;
    return p;
}

void _hash_pair_destroy(HashTableItem *p)
{
    free(p);
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
    HashTable *hash_tbl = calloc(1, sizeof(HashTable));

    hash_tbl->table_size = table_size;
    hash_tbl->hash_fn = hash_fn;
    hash_tbl->cmp_fn = cmp_fn;
    hash_tbl->buckets = calloc(table_size, sizeof(ForwardList *));
    hash_tbl->n_elements = 0;

    return hash_tbl;
}

void *hash_table_set(HashTable *h, void *key, void *val)
{
    int idx = h->hash_fn(h, key);
    HashTableItem *item;

    if(h->buckets[idx]){
        Node *n = h->buckets[idx]->head;

        while(n){
            HashTableItem *hti = (HashTableItem *) n->value;

            if(!h->cmp_fn(hti->key, key)){
                void *v = hti->val;
                hti->val = val;
                
                return v;
            }
            n = n->next;
        }
        
        item = _hash_pair_construct(key, val);
        forward_list_push_back(h->buckets[idx], item);

    } else{
        h->buckets[idx] = forward_list_construct();
        item = _hash_pair_construct(key, val);
        forward_list_push_back(h->buckets[idx], item);

    }
    h->n_elements++;

    return NULL;
}

void *hash_table_get(HashTable *h, void *key)
{
    int idx = h->hash_fn(h, key);

    if(h->buckets[idx]){
        Node *n = h->buckets[idx]->head;

        while(n){
            HashTableItem *item = (HashTableItem *) n->value;
            
            if(!h->cmp_fn(item->key, key))
                return item->val;
            
            n = n->next;
        }
    }

    return NULL;
}

void *hash_table_pop(HashTable *h, void *key)
{
    int idx = h->hash_fn(h, key);
    Node *prev = NULL, *next;

    if(h->buckets[idx]){
        Node *n = h->buckets[idx]->head;

        while(n){
            next = n->next;
            HashTableItem *item = n->value;

            if(!h->cmp_fn(key, item->key)){
                
                if(prev)
                    prev->next = next;
                else
                    h->buckets[idx]->head = next;
                
                n->next = NULL;

                if(!h->buckets[idx]->head)
                    free(h->buckets[idx]);
                    
                return n;
            }            

            prev = n;
            n = next;
        }
    }
    return NULL;
}

int hash_table_size(HashTable *h)
{
    return h->table_size;
}

int hash_table_num_elems(HashTable *h)
{
    return h->n_elements;
}

void hash_table_destroy(HashTable *h)
{
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->buckets[i] != NULL)
        {
            Node *n = h->buckets[i]->head;

            while (n != NULL)
            {
                HashTableItem *pair = n->value;
                free(pair->key);
                free(pair->val);

                _hash_pair_destroy(pair);
                n = n->next;
            }

            forward_list_destroy(h->buckets[i]);
        }
    }

    free(h->buckets);
    free(h);
}