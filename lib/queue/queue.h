#ifndef QUEUE_H
#define QUEUE_0

#define OVERFLOW 2001
#define UNDERFLOW 2002

typedef struct Queue {
    Vertex ** list;
    size_t head;
    size_t tail;
    size_t msize;
} Queue;


int push(Queue * q, Vertex * v);
int pop(Queue * q, Vertex ** res);

#endif
