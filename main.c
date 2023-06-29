#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"

int main(){
    Deque *d = deque_construct();
    Cel *c;

    int qtd, x, y;
    char oq[20];
    qtd = 2;
    // scanf("%d", &qtd);

    for(int i = 0; i < qtd; i++){
        scanf("%s", &oq);

        if(!strcmp(oq, "POP_FRONT")){
            c = (Cel*) deque_pop_front(d);
            cel_print(c);

        } else if(!strcmp(oq, "POP_BACK")){
            c = (Cel*) deque_pop_back(d);
            cel_print(c);

        } else{

            scanf("%d %d", &x, &y);
            c = cel_construct(x, y);
            cel_print(c);

            if(!strcmp(oq, "PUSH_FRONT")){
                deque_push_front(d, c);

            } else{
                deque_push_back(d, c);

            }
        }
    }

    return 0;
}

/*
POP_FRONT
POP_BACK
POP_BACK
PUSH_FRONT 74 28
POP_FRONT
PUSH_BACK 80 50
POP_BACK
*/