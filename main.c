#include <stdio.h>
#include <stdlib.h>

#include "deque.h"

int main(){
    Deque *q = deque_construct();
    void *v = NULL;
    deque_push_back(q, v);
    deque_push_front(q, v);

    return 0;
}