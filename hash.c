
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

HashTableItem *_hash_pair_construct(int key, void *val)
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

void hash_table_set(HashTable *h, int key, void *val)
{
    int idx = h->hash_fn(h, key);
    HashTableItem *item = _hash_pair_construct(key, val);

    if(h->buckets[idx]){
        HashTableItem *n = (HashTableItem*) forward_list_find(h->buckets[idx], key, h->cmp_fn);

        if(n){
            n->val = val;

        } else {
            forward_list_push_back(h->buckets[idx], (data_type*) item);
        }

    } else{
        h->buckets[idx] = forward_list_construct();
        forward_list_push_back(h->buckets[idx], (data_type*) item);

    }
}

void *hash_table_get(HashTable *h, int key)
{
    int idx = h->hash_fn(h, key);

    if(h->buckets[idx]){
        HashTableItem *item = (HashTableItem*) forward_list_find(h->buckets[idx], key, h->cmp_fn);

        if(item) return item;

    }

    return NULL;
}

void *hash_table_pop(HashTable *h, int key)
{
    // ************************************
    // TODO!!
    // ************************************

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
                _hash_pair_destroy(pair);
                n = n->next;
            }

            forward_list_destroy(h->buckets[i]);
        }
    }

    free(h->buckets);
    free(h);
}