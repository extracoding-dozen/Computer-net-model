#include "graph.h"
#include "errors.h"
#include "./queue/queue.h"
#include "./pairs/pairs.h"
#include <limits.h>

Vertex * make_vertex(char * key, uns_i port) {
    Vertex * v = calloc(1, sizeof(Vertex));
    v -> name = key;
    v -> port = port;
    v -> edlist = NULL;
    return v;
}

Graph create_graph(size_t msize) {
    Graph g;
    g.msize = msize;
    g.csize = 0;
    g.vertex_list = calloc(msize, sizeof(Vertex *));
    return g;
}

int paste_vertex(Graph * g, char * name, uns_i port) {
    if (!g) return MEMORY_ERROR;
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, name) == 0) {
            free(name);
            return NAME_DEBLOWING;
        }
    }
    if (g -> csize == g -> msize) {
        g -> msize *= 2;
        g -> vertex_list = realloc(g -> vertex_list, g -> msize * sizeof(Vertex *));
    }
    (g -> vertex_list)[g -> csize] = make_vertex(name, port);
    g -> csize += 1;
    return CORRECT;
}


Vertex * find_computer(Graph * g, char * name) {
    size_t csize = g -> csize;
    for (size_t i = 0; i < csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, name)== 0) return (g -> vertex_list)[i];
    }
    return NULL;
}


Edge * make_edge(uns_i * allowed, size_t size, Vertex * to) {
    if (!to) return NULL;
    Edge * e = calloc(1, sizeof(Edge));
    e -> allowed_ports = allowed;
    e -> size = size;
    e -> to = to;
    return e;
}


int find_edge(Graph * g, char * from, char * to) {
    for (size_t i = 0; i < g -> csize; i++) {
        Vertex * v = (g -> vertex_list)[i];
        if (strcmp(v -> name, from) == 0) {
            Edge * e = v -> edlist;
            while (e) {
                if (strcmp(e -> to -> name, to) == 0) return 1;
                e = e -> next;
            }
        }
    }
    return 0;
}


int paste_edge(Graph * g, uns_i * allowed, size_t size, char * from, char * to) {
    if (!g) return MEMORY_ERROR;
    if (find_edge(g, from, to)) return EDGE_DEBLOWING;
    int diff = strcmp(from, to);
    Vertex * start = find_computer(g, from);
    Vertex * end = find_computer(g, to);
    if (!start || !end) return UNCOMPLETE_EGDE;
    Edge * pasted = make_edge(allowed, size, end);
    if (diff != 0) {
        Edge * rev = make_edge(allowed, size, start);
        rev -> next = end -> edlist;
        end -> edlist = rev;
    }
    pasted -> next = start -> edlist;
    start -> edlist = pasted;
    return CORRECT;

}

void write_edge_info(Edge * e, FILE * out) {
    uns_i * ar = e -> allowed_ports;
    for (size_t i = 0; i < e -> size; i++) {
        fprintf(out, "%u\\n", ar[i]);
    }
}


int find(Pair * plist, size_t len, Pair el) {
    for (size_t i = 0; i < len; i++) {
        if (compare(plist[i], el)) return 1;
    }
    return 0;
}


void visuzlize(Graph * g) {
    FILE * out = fopen("graphout.dot", "wt");
    fprintf(out, "graph G {\n");
    size_t plist_size = g -> csize * 2 + 1;
    Pair * plist = calloc(g -> csize * 2 + 1, sizeof(Pair));
    size_t past_ind = 0;
    
    for (size_t i = 0; i < g -> csize;  i++) {
        fprintf(out, "\t%s [label = \"", (g -> vertex_list)[i] -> name);
        fprintf(out, " Username = %s\\n Port = %u", (g -> vertex_list)[i] -> name, \
        (g -> vertex_list)[i] -> port);
        fprintf(out, "\"];\n");
    }
    fprintf(out, "\n");
    for (size_t i = 0; i < g -> csize; i++) {
        Vertex * v = (g -> vertex_list)[i];
        Edge * e = v -> edlist;
        while (e) {
            Pair printer = create_pair(v, e -> to);
            if (!find(plist, past_ind, printer)) {
                fprintf(out, "%s -- %s [label = \"[", v -> name, e -> to -> name);
                write_edge_info(e, out);
                fprintf(out, "]\"];\n");
                Pair res = create_pair(e -> to, v);
                if (past_ind == plist_size) {
                    plist = realloc(plist, plist_size * 2 * sizeof(Pair));
                    plist_size *= 2;
                }
                plist[past_ind] = res;
                past_ind++;
            }
            e = e -> next;
        }
    }
    free(plist);
    fprintf(out, "}");
    fclose(out);

}

int find_array(uns_i ** q, size_t csize, uns_i * ar) {
    for (size_t i = 0; i < csize; i++) {
        if (q[i] == ar) return 1;
    }
    return 0;
}

int clean_graph(Graph * g) {
    size_t csize = 0;
    size_t msize = 30;
    uns_i ** q = calloc(msize, sizeof(uns_i*));
    for (size_t i = 0; i < g -> csize; i++) {
        Edge * e = (g -> vertex_list)[i] -> edlist;
        Edge * back = NULL; 
        while (e) {
            if (back) {
                if (!find_array(q, csize, back -> allowed_ports)) {
                    if (csize == msize) {
                        msize *= 2;
                        q = realloc(q, sizeof(uns_i * ) * msize);
                    }
                    q[csize] = back -> allowed_ports;
                    csize++;
                }
                free(back);
            }
            back = e;
            e = e -> next;
        }
        if (back) {
            if (!find_array(q, csize, back -> allowed_ports)) {
                if (csize == msize) {
                    msize *= 2;
                    q = realloc(q, sizeof(uns_i * ) * msize);
                }
                q[csize] = back -> allowed_ports;
                csize++;
            }
            free(back);
        }
        free((g -> vertex_list)[i] -> name);
        free((g -> vertex_list)[i]);
    }
    free(g -> vertex_list);
    for (size_t i = 0; i < csize; i++) {
        free(q[i]);
    }
    free(q);
    return CORRECT;
}


int remove_edge(Graph * g, char * from, char * to) {
    if (!g) return MEMORY_ERROR;
    if (!find_edge(g, from, to)) return NO_EDGE;
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, from) == 0) {
            Vertex * v = (g -> vertex_list)[i];
            Edge * e = v -> edlist;
            if (strcmp(e -> to -> name, to) == 0) {
                v -> edlist = e -> next;
                if (e -> allowed_ports) free(e -> allowed_ports);
                free(e);
            }
            else {
                Edge * back = e;
                e = e -> next;
                while (strcmp(e -> to -> name, to) != 0) {
                    back = e;
                    e = e -> next;
                }
                back -> next = e -> next;
                if (e -> allowed_ports) free(e -> allowed_ports);
                free(e);
            }
        }
    }
    if (strcmp(from, to) != 0) {
        char * tmp = from;
        from = to;
        to = tmp;
        for (size_t i = 0; i < g -> csize; i++) {
            if (strcmp((g -> vertex_list)[i] -> name, from) == 0) {
                Vertex * v = (g -> vertex_list)[i];
                Edge * e = v -> edlist;
                if (strcmp(e -> to -> name, to) == 0) {
                    v -> edlist = e -> next;
                    //if (e -> allowed_ports) free(e -> allowed_ports);
                    free(e);
                    return CORRECT;
                }
                Edge * back = e;
                e = e -> next;
                while (strcmp(e -> to -> name, to) != 0) {
                    back = e;
                    e = e -> next;
                }
                back -> next = e -> next;
                //if (e -> allowed_ports) free(e -> allowed_ports);
                free(e);
                return CORRECT;
            }
        }
    }
    return CORRECT;
}

void kill_edges(Graph * g, char * from, Edge * start) {
    if (start) {
        if (start -> next) {
            kill_edges(g, from, start -> next);
            start -> next = NULL;
        }
        remove_edge(g, from, start -> to -> name);
    }
}

int remove_vertex(Graph * g, char * name) {
    if (!g || !name) return MEMORY_ERROR;
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, name) == 0) {
            kill_edges(g, (g -> vertex_list)[i] -> name, (g -> vertex_list)[i] -> edlist);
            free((g -> vertex_list)[i] -> name);
            free((g -> vertex_list)[i]);
            (g -> vertex_list)[i] = (g -> vertex_list)[g -> csize - 1];
            g -> csize -= 1;
            return CORRECT;
        }   
    }
    return NO_ELEM;
}


int change_vertex(Graph * g, char * name, char * new_name, uns_i * new_port) {
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, name) == 0) {
            Vertex * v = (g -> vertex_list)[i];
            if (new_name) {
                free(v -> name);
                v -> name = new_name;
            }
            if (new_port)  v -> port = *new_port;
            return CORRECT;
        }
    }
    return NO_ELEM;
}


int change_edge(Graph * g, char * from, char * to, size_t new_size, uns_i * new_ports) {
    char found = 0;
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, from) == 0) {
            Edge * e = (g -> vertex_list)[i] -> edlist;
            while (e) {
                if (strcmp(e -> to -> name, to) == 0) {
                    if (e -> allowed_ports) free(e -> allowed_ports);
                    e -> size = new_size;
                    e -> allowed_ports = new_ports;
                    found = 1;
                    break;
                }
                e = e -> next;
            }
            if (found) break;
        }
    }
    if (!found) return NO_EDGE;
    if (strcmp(from, to) == 0) return CORRECT;
    char * tmp = from;
    from = to;
    to = tmp;
    found = 0;
    for (size_t i = 0; i < g -> csize; i++) {
        if (strcmp((g -> vertex_list)[i] -> name, from) == 0) {
            Edge * e = (g -> vertex_list)[i] -> edlist;
            while (e) {
                if (strcmp(e -> to -> name, to) == 0) {
                    e -> size = new_size;
                    e -> allowed_ports = new_ports;
                    found = 1;
                    return CORRECT;
                }
                e = e -> next;
            }
            if (found) break;
        }
    }
    return CORRECT;
}



int find_port(uns_i * portlist, size_t size, uns_i port) {
    for (size_t i = 0; i < size; i++) {
        if (portlist[i] == port) return 1;
    }
    return 0;
}


void DFS(Vertex * start, Vertex ** list, size_t * csize) {
    Edge * e = start -> edlist;
    start -> color = 1;
    list[*csize] = start;
    *csize += 1;
    while (e) {
        if (find_port(e -> allowed_ports, e -> size, start -> port) && \
        find_port(e -> allowed_ports, e -> size, e -> to -> port) && e -> to -> color != 1) {
            DFS(e -> to, list, csize);
        }
        e = e -> next;
    }
}


Vertex ** find_all_path(Graph * g, char * name) {
    for (size_t i = 0; i < g -> csize; i++) {
        (g -> vertex_list)[i] -> color = 0;
    }
    size_t csize = 0;
    Vertex ** res = calloc(g -> csize + 1, sizeof(Vertex * ));
    res[g -> csize] = NULL;
    Vertex * start = find_computer(g, name);
    DFS(start, res, &csize);
    return res;
}

int min(int a, int b) {
    if (a > b) return b;
    return a;
}

void BFS(Vertex * start, uns_i port, Queue * q) {
    start -> color = 1;
    Edge * e = start -> edlist;
    while (e) {
        if (start -> port == port && e -> to -> port == port && \
        find_port(e -> allowed_ports, e -> size, port) && e -> to -> color == 0) {
            e -> to -> path = min(start -> path + 1, e -> to -> path);
            push(q, e -> to);
        }
        e = e -> next;
    }
    Vertex * res;
    if (pop(q, &res) == CORRECT) {
        BFS(res, port, q);
    }
}

void unpack_path(Vertex * end, Vertex * start, Vertex ** res, size_t * ind) {
    if (strcmp(end -> name, start -> name) != 0) {
        Edge * e = end -> edlist;
        while (e) {
            if (e -> to -> path == end -> path - 1) {
                unpack_path(e -> to, start, res, ind);
                res[*ind] = end;
                *ind += 1;
                break;
            }
            e = e -> next;
        }
    }
    else {
        res[*ind] = end;
        *ind += 1;
    }
}

Vertex ** find_min_path(Graph * g, char * from, char * to, uns_i port) {
    for (size_t i = 0; i < g -> csize; i++) {
        (g -> vertex_list)[i] -> path = INT_MAX;
        (g -> vertex_list)[i] -> color = 0;
    }
    Vertex * start = find_computer(g, from);
    start -> path = 0;
    Queue q;
    q.list = calloc(g -> csize, sizeof(Vertex *));
    q.msize = (g -> csize);
    q.head = 0;
    q.tail = 0;
    BFS(start, port, &q);
    free(q.list);
    Vertex * end = find_computer(g, to);
    if (end -> color == 0) return NULL;
    size_t ind = 0;
    Vertex ** result = calloc(end -> path + 2, sizeof(Vertex *));
    result[end -> path + 1] = NULL;
    unpack_path(end, start, result, &ind);
    return result;
}

char * freadline(FILE * in) {
    size_t buflen = 80;
    size_t ind = 0;
    char have;
    char * buf = calloc(buflen, sizeof(char));
    fscanf(in, "%c", &have);
    if (have == '\n') fscanf(in, "%c", &have);
    while (have != '\n') {
        if (ind == buflen) {
            buflen *= 2;
            buf = realloc(buf, buflen * sizeof(char));
        }
        buf[ind] = have;
        ind++;
        fscanf(in, "%c", &have);
    }
    buf = realloc(buf, (ind + 1) * sizeof(int));
    buf[ind] = '\0';
    return buf;
}


int read_graph(Graph * g, char * filename) {
    FILE * in = fopen(filename, "rt");
    size_t cnt;
    fscanf(in, "%lu", &cnt);
    g -> msize = cnt;
    g -> csize = 0;
    g -> vertex_list = calloc(cnt, sizeof(Vertex *));
    char * name;
    char * from;
    char * to;
    uns_i port;
    size_t size;

    for (size_t i = 0; i < cnt; i++) {
        name = freadline(in);
        fscanf(in, "%u", &port);
        paste_vertex(g, name, port);
    }
    fscanf(in, "%lu", &cnt);
    for (size_t i = 0; i < cnt; i++) {
        from = freadline(in);
        to = freadline(in);
        fscanf(in, "%lu", &size);
        uns_i * ar = calloc(size, sizeof(uns_i));
        for (size_t k = 0; k < size; k++) {
            fscanf(in, "%u", ar + k);
        }
        paste_edge(g, ar, size, from, to);
        free(from);
        free(to);
    }
    fclose(in);
    return CORRECT;

}

Vertex *** split_graph(Graph * g) {
    Vertex *** res = calloc(g -> csize, sizeof(Vertex **));
    size_t csize = 0;
    for (size_t i = 0; i < g -> csize; i++) {
        (g -> vertex_list)[i] -> color = 0;
    }
    for (size_t i = 0; i < g -> csize; i++) {
        csize = 0;
        if ((g -> vertex_list)[i] -> color == 0) {
            res[i] = calloc(g -> csize + 1, sizeof(Vertex *));
            DFS((g -> vertex_list)[i], res[i], &csize);
        }
    }
    return res;
}


int print_sent_list(Graph * g) {
    if (!g) return MEMORY_ERROR;
    size_t size = g -> csize;
    for (size_t i = 0; i < size; i++) {
        Vertex ** v = g -> vertex_list;
        printf("%s: ", v[i] -> name);
        Edge * e = v[i] -> edlist;
        while (e) {
            if (e -> next == NULL) {
                printf("%s\n", e -> to -> name);
            }
            else printf("%s, ", e -> to -> name);
            e = e -> next;
        }
    }
    return CORRECT;
}
