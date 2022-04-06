#ifndef BIPARTITY_CHECK_H_INCLUDED
#define BIPARTITY_CHECK_H_INCLUDED

#include "graph.h"

int check_neighbours(adj_node* ptr, int* color);
int bipartity_check_dfs(graph* g, int** a, int* k, int** b, int* l);
int bipartity_check_bfs(graph* g, int** a, int* k, int** b, int* l);
int is_bipartite(graph* g, int* color);

#endif // BIPARTITY_CHECK_H_INCLUDED