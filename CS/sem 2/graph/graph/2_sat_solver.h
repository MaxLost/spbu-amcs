#ifndef SAT2_SOLVER_H_INCLUDED
#define SAT2_SOLVER_H_INCLUDED

#include "graph.h"
#include "scc_partition.h"

struct sat_node {
	int l;
	int r;
	sat_node* next;
};

typedef struct sat_node sat_node;

sat_node* sat_node_create(int l, int r);

sat_node* get_expression(char *str);

int sat_solver(sat_node* expr);

#endif SAT2_SOLVER_H_INCLUDED