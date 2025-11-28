#include "../graph.h"
#include "pairs.h"


Pair create_pair(Vertex * first, Vertex * second) {
    Pair p;
    p.first = first;
    p.second = second;
    return p;
}


int compare(Pair p1, Pair p2) {
    if (strcmp(p1.first -> name, p2.first -> name) == 0 && \
    strcmp(p1.second -> name, p2.second -> name) == 0) {
        return 1;
    }
    return 0;
}