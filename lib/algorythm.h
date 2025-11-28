#ifndef ALGORYTHM_H
#define ALGORYTHM_0

#include <readline/readline.h>
#include <readline/history.h>
#include "errors.h"

int print_sent_list(Graph * g);
Vertex *** split_graph(Graph * g);
int read_graph(Graph * g, char * filename);
Vertex ** find_min_path(Graph * g, char * from, char * to, uns_i port);
Vertex ** find_all_path(Graph * g, char * name);
int change_edge(Graph * g, char * from, char * to, size_t new_size, uns_i * new_ports);
int change_vertex(Graph * g, char * name, char * new_name, uns_i * new_port);
int remove_vertex(Graph * g, char * name);
int remove_edge(Graph * g, char * from, char * to);
int clean_graph(Graph * g);
void visuzlize(Graph * g);
int paste_edge(Graph * g, uns_i * allowed, size_t size, char * from, char * to);
Graph create_graph(size_t msize);
int paste_vertex(Graph * g, char * name, uns_i port);

#endif