#include "../graph.h"
#include "queue.h"
#include "../errors.h"



int push(Queue * q, Vertex * v) {
    if (!q) return MEMORY_ERROR;
    if (q -> head == q -> tail && (q -> list)[q -> tail] != NULL) return OVERFLOW;
    (q -> list)[q -> head] = v;
    q -> head = (q -> head + 1) % (q -> msize);
    return CORRECT;
}

int pop(Queue * q, Vertex ** result) {
    if (!q) return MEMORY_ERROR;
    if (q -> head == q -> tail) return UNDERFLOW;
    size_t ind = q -> tail;
    q -> tail = (q -> tail + 1) % (q -> msize);
    *result = (q -> list)[ind];
    return CORRECT;
}


