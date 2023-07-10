
#include "stack.h"

struct Stack{
    Deque *deque;
    int size;
};

Stack *stack_construct(){
    Stack *s = malloc(sizeof(Stack));

    s->deque = deque_construct();
    s->size = 0;

    return s;
}

void stack_push(Stack *stack, void *data){

    deque_push_back(stack->deque, data);
    stack->size++;
}

bool stack_empty(Stack *stack){
    return !stack->size;
}

void *stack_pop(Stack *stack){

    void *v = deque_pop_back(stack->deque);
    if(stack->size) stack->size--;

    return v;
}

void stack_destroy(Stack *stack){
    deque_destroy(stack->deque);
    free(stack);
}