#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include "str_input.h"
#include "list.h"

struct adj_node {
    int num;
    node* arc_list;
    adj_node* next;
};

typedef struct adj_node adj_node;

typedef struct {
    int n;
    adj_node* adj_list;
} graph;

graph* graph_init(int n);

graph* graph_get(int n);

void graph_free(graph* g);

void add_arc(graph* g, int origin, int destination);

void add_edge(graph* g, int v, int w);

void delete_arc(graph* g, int origin, int destination);

void delete_edge(graph* g, int v, int w);

void graph_print(graph* g);

int check_neighbours(adj_node* ptr, int* color);

#endif // GRAPH_H_INCLUDED