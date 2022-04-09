#ifndef GRAPH_FILE_INPUT_H_INCLUDED
#define GRAPH_FILE_INPUT_H_INCLUDED

#include "graph.h"

graph* graph_read(const char* path);

int graph_write(graph* g, const char* path);

#endif // GRAPH_FILE_INPUT_H_INCLUDED