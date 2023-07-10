
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "stack.h"
#include "queue.h"

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

int celula_hash(HashTable *h, void *key)
{
    Celula *c = (Celula *)key;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c->x * 83) ^ (c->y * 97)) % hash_table_size(h);
}

int celula_cmp(void *c1, void *c2)
{
    Celula *a = (Celula *)c1;
    Celula *b = (Celula *)c2;

    if (a->x == b->x && a->y == b->y)
        return 0;
    else
        return 1;
}

ResultData a_star(Labirinto *l, Celula inicio, Celula fim){

    ResultData result = _default_result();
    int max_path_length = labirinto_n_colunas(l) * labirinto_n_linhas(l);
    result.caminho = malloc( max_path_length * sizeof(Celula) );

    Celula atual = inicio, *c_inicio = malloc(sizeof(Celula)), *c_pop;
    c_inicio->prev = NULL;
    c_inicio->x= atual.x;
    c_inicio->y= atual.y;
    c_pop = c_inicio;

    HashTable *ht = hash_table_construct(19, celula_hash, celula_cmp);
    Heap *heap = heap_construct(ht);
    double g = calcula_custo(&atual, c_inicio), h = calcula_custo(&atual, &fim);
    c_inicio->g = g;
    c_inicio->h = h;
    heap_push(heap, c_inicio, c_inicio->g + c_inicio->h);
    void *v;


    while( 1 ){

        c_pop = heap_pop(heap);
        atual.x = c_pop->x;
        atual.y = c_pop->y;
        atual.g = c_pop->g;
        atual.h = c_pop->h;
        if( atual.x == fim.x && atual.y == fim.y )
            break;

            printf("\nCelula atual: %d %d\ng(n) e h(n) : %.2lf %.2lf\n", atual.x, atual.y, atual.g, atual.h);
        // Adiciona celulas na fronteira de acordo com a celula atual
        for(int i = 1; i <= 8; i++){
            Celula c = celula_return(atual, i);
            if( celula_valid(c, l) && (labirinto_obter(l, c.x, c.y) == LIVRE || labirinto_obter(l, c.x, c.y) == FIM || labirinto_obter(l, c.x, c.y) == LIVRE) ){
                Celula *c_aux = (Celula *) malloc(sizeof(Celula));
                c_aux->x = c.x;
                c_aux->y = c.y;
                c_aux->prev = c_pop;
                c_aux->g = calcula_custo(&c, &inicio);
                c_aux->h = calcula_custo(&c, &fim);
                v = heap_push(heap, c_aux, c_aux->g + c_aux->h);
                if(v)
                    free(v);
                labirinto_atribuir(l, c.x, c.y, FRONTEIRA);
            }
        }
        
        result.nos_expandidos++;

        if(heap_empty(heap)){
            break;
        }

    }

    if(atual.x == fim.x && atual.y == fim.y)
        result.sucesso++;

        Celula *aux = c_pop;
        while(aux){
            aux = aux->prev;
            result.tamanho_caminho++;
        }
        int tam = result.tamanho_caminho;
    while(c_pop && result.sucesso){


        result.caminho[--tam] = atual;

        labirinto_atribuir(l, atual.x, atual.y, CAMINHO);

        float dy, dx;
        if(c_pop->prev){
            dx = c_pop->x - c_pop->prev->x;
            dy = c_pop->y - c_pop->prev->y;
            result.custo_caminho += calcula_custo(c_pop, c_pop->prev);
        }

        c_pop = c_pop->prev;
        if(c_pop){
            atual.x = c_pop->x;
            atual.y = c_pop->y;
        }
    }


    return result;
}

ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim){
    
    ResultData result = _default_result();
    int max_path_length = labirinto_n_colunas(l) * labirinto_n_linhas(l);
    result.caminho = malloc( max_path_length * sizeof(Celula) );

    Celula atual = inicio, *c_inicio = malloc(sizeof(Celula)), *c_pop;
    c_inicio->prev = NULL;
    c_inicio->x= atual.x;
    c_inicio->y= atual.y;
    c_pop = c_inicio;
    
    Queue *q = queue_construct(), *clean = queue_construct();
    queue_push(q, c_pop);

    while( 1 ){

        c_pop = queue_pop(q);
        atual.x = c_pop->x;
        atual.y = c_pop->y;
        if( atual.x == fim.x && atual.y == fim.y )
            break;

        // Adiciona celulas na fronteira de acordo com a celula atual
        for(int i = 1; i <= 8; i++){
            Celula c = celula_return(atual, i);
            if( celula_valid(c, l) && (labirinto_obter(l, c.x, c.y) == LIVRE || labirinto_obter(l, c.x, c.y) == FIM) ){
                Celula *c_aux = (Celula *) malloc(sizeof(Celula));
                c_aux->x = c.x;
                c_aux->y = c.y;
                c_aux->prev = c_pop;
                queue_push(q, c_aux);
                queue_push(clean, c_aux);
                labirinto_atribuir(l, c.x, c.y, FRONTEIRA);
            }
        }
        
        result.nos_expandidos++;

        if(queue_empty(q)){
            break;
        }

    }

    if(atual.x == fim.x && atual.y == fim.y)
        result.sucesso++;

        Celula *aux = c_pop;
        while(aux){
            aux = aux->prev;
            result.tamanho_caminho++;
        }
        int tam = result.tamanho_caminho;
    while(c_pop && result.sucesso){


        result.caminho[--tam] = atual;

        labirinto_atribuir(l, atual.x, atual.y, CAMINHO);

        float dy, dx;
        if(c_pop->prev){
            result.custo_caminho += calcula_custo(c_pop, c_pop->prev);
        }

        c_pop = c_pop->prev;
        if(c_pop){
            atual.x = c_pop->x;
            atual.y = c_pop->y;
        }
    }

    while(!queue_empty(clean)){
        Celula *c = queue_pop(clean), *aux;
        aux = c->prev;
        if(c){
            free(c);
        }
    }
    free(c_inicio);

    queue_destroy(clean);
    queue_destroy(q);

    return result;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();
    int max_path_length = labirinto_n_colunas(l) * labirinto_n_linhas(l);
    result.caminho = malloc( max_path_length * sizeof(Celula) );

    Celula atual = inicio, *c_inicio = malloc(sizeof(Celula)), *c_pop;
    c_inicio->prev = NULL;
    c_inicio->x= atual.x;
    c_inicio->y= atual.y;
    c_pop = c_inicio;
    
    Stack *q = stack_construct(), *clean = stack_construct();
    stack_push(q, c_pop);

    while( 1 ){

        c_pop = stack_pop(q);
        atual.x = c_pop->x;
        atual.y = c_pop->y;
        if( atual.x == fim.x && atual.y == fim.y )
            break;

        // Adiciona celulas na fronteira de acordo com a celula atual
        for(int i = 1; i <= 8; i++){
            Celula c = celula_return(atual, i);
            if( celula_valid(c, l) && (labirinto_obter(l, c.x, c.y) == LIVRE || labirinto_obter(l, c.x, c.y) == FIM) ){
                Celula *c_aux = (Celula *) malloc(sizeof(Celula));
                c_aux->x = c.x;
                c_aux->y = c.y;
                c_aux->prev = c_pop;
                stack_push(q, c_aux);
                stack_push(clean, c_aux);
                labirinto_atribuir(l, c.x, c.y, FRONTEIRA);
            }
        }
        
        result.nos_expandidos++;

        if(stack_empty(q)){
            break;
        }

    }

    if(atual.x == fim.x && atual.y == fim.y)
        result.sucesso++;

        Celula *aux = c_pop;
        while(aux){
            aux = aux->prev;
            result.tamanho_caminho++;
        }
        int tam = result.tamanho_caminho;
    while(c_pop && result.sucesso){


        result.caminho[--tam] = atual;

        labirinto_atribuir(l, atual.x, atual.y, CAMINHO);

        float dy, dx;
        if(c_pop->prev){
            dx = c_pop->x - c_pop->prev->x;
            dy = c_pop->y - c_pop->prev->y;
            result.custo_caminho += calcula_custo(c_pop, c_pop->prev);
        }

        c_pop = c_pop->prev;
        if(c_pop){
            atual.x = c_pop->x;
            atual.y = c_pop->y;
        }
    }

    while(!stack_empty(clean)){
        Celula *c = stack_pop(clean), *aux;
        aux = c->prev;
        if(c){
            free(c);
        }
    }
    free(c_inicio);

    stack_destroy(clean);
    stack_destroy(q);

    return result;
}

ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim)
{
    int max_path_length = 0;
    float dx, dy;

    ResultData result = _default_result();

    max_path_length = abs(fim.x - inicio.x) + abs(fim.y - inicio.y);
    printf("Max : %d\n", max_path_length);
    result.caminho = (Celula *)malloc(sizeof(Celula) * max_path_length);
    result.sucesso = 1;

    Celula atual = inicio;
    result.caminho[result.tamanho_caminho++] = atual;
    result.nos_expandidos++;

    while ((atual.x != fim.x) || (atual.y != fim.y))
    {
        dx = fim.x - atual.x;
        dy = fim.y - atual.y;

        if (dx != 0)
            dx /= fabs(dx);

        if (dy != 0)
            dy /= fabs(dy);

        atual.x += (int)dx;
        atual.y += (int)dy;

        if (labirinto_obter(l, atual.y, atual.x) == OCUPADO || (atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        {
            result.sucesso = 0;
            free(result.caminho);
            result.caminho = NULL;
            return result;
        }

        result.caminho[result.tamanho_caminho++] = atual;
        result.nos_expandidos++;
        result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return result;
}

Celula celula_return(Celula atual, int i){
    Celula c;
    c.x = atual.x;
    c.y = atual.y;

    // Adiciona ou decrementa eixo x
    switch (i){
        case 1:
        case 2:
        case 8:
            c.x--;
            break;

        case 4:
        case 5:
        case 6:
            c.x++;        
            break;
    }

    // Adiciona o decrementa eixo y
    switch (i){
        case 6:
        case 7:
        case 8:
            c.y--;
            break;

        case 2:
        case 3:
        case 4:
            c.y++;        
            break;
    }

    return c;
}

bool celula_valid(Celula c, Labirinto *l){
    if(c.x >= 0 && c.x < labirinto_n_linhas(l))
        if(c.y >= 0 && c.y < labirinto_n_colunas(l))
            return 1;

    return 0;
}

double calcula_custo(Celula *c1, Celula *c2){
    double dx, dy;
    dx = c1->x - c2->x;
    dy = c1->y - c2->y;
    return sqrt((dx*dx) + (dy*dy));
}
