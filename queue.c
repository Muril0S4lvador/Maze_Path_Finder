
#include "queue.h"

struct Queue{
    Deque *deque;
    int size;
};

Queue *queue_construct(){
    Queue *q = malloc(sizeof(Queue));

    q->deque = deque_construct();
    q->size = 0;

    return q;
}

void queue_push(Queue *queue, void *data){

    deque_push_front(queue->deque, data);
    queue->size++;
}

bool queue_empty(Queue *queue){
    return !queue->size;
}

int queue_size(Queue *queue){
    return queue->size;
}

void *queue_pop(Queue *queue){
    void *v = deque_pop_back(queue->deque);
    if(queue->size) queue->size--;

    return v;
}

void queue_destroy(Queue *queue){
    deque_destroy(queue->deque);
    free(queue);
}