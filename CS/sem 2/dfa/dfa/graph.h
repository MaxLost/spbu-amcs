#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
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

adj_node* find_adj_node(int v, graph* g);

void graph_free(graph* g);

node* find_arc(graph* g, int origin, int destination, int condition);
node* find_arc(graph* g, int origin, int condition);

void add_arc(graph* g, int origin, int destination, int condition);
void delete_arc(graph* g, int origin, int destination, int condition);

void graph_print(graph* g);

#endif // GRAPH_H_INCLUDED