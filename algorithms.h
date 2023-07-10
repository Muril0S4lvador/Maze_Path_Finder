
#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "labirinto.h"
#include "types.h"
#include "heap.h"


typedef struct Celula
{
    int x;
    int y;
    double g;
    double h;
    struct Celula *prev;
} Celula;

typedef struct
{
    Celula *caminho;
    double custo_caminho;
    int tamanho_caminho;
    int nos_expandidos;
    int sucesso;
} ResultData;

ResultData a_star(Labirinto *l, Celula inicio, Celula fim);
ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim);
ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim);

Celula celula_return(Celula atual, int i);
bool celula_valid(Celula c, Labirinto *l);
double calcula_custo(Celula *c1, Celula *c2);

int celula_cmp(void *c1, void *c2);
int celula_hash(HashTable *h, void *key);
// algoritmo bobo de teste que tenta ir em linha reta do inicio ao fim
// e retorna impossível se encontrar um obstáculo
ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim);

#endif