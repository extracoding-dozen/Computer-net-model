#ifndef GRAPH_H
#define GRAPH_0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int uns_i;

struct Egde;

typedef struct Vertex {
    char * name;
    uns_i port;
    char color;
    int path;
    struct Edge * edlist;
} Vertex;

typedef struct Edge {
    Vertex * to;
    uns_i * allowed_ports;
    size_t size;
    struct Edge * next;
} Edge;

typedef struct Graph {
    size_t csize;
    size_t msize;
    Vertex ** vertex_list;
} Graph;

#endif
