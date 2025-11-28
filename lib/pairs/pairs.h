#ifndef PAIRS_H
#define PAIRS_0

typedef struct Pair {
    Vertex * first;
    Vertex * second;
} Pair;


int compare(Pair p1, Pair p2);
Pair create_pair(Vertex * fisrt, Vertex * second);

#endif