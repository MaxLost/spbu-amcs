#include <stdio.h>
#include "graph.h"
#include "bipartity_check.h"
#include "topological_sort.h"
#include "file_input.h"
#include "scc_partition.h"
#include "2_sat_solver.h"

#define CUR_DIR "..\\..\\..\\..\\graph\\"

void bipartite_test(graph* x) {

	int* color = (int*)calloc(x->n, sizeof(int));
	if (color) {
		if (is_bipartite(x, color)) {
			printf("Graph is not biparted");
			return;
		}
		else {
			printf("1st part: ");
			for (int i = 0; i < x->n; i++) {
				if (color[i] == 1) printf("%d ", i);
			}
			printf("\n2nd part: ");
			for (int i = 0; i < x->n; i++) {
				if (color[i] == 2) printf("%d ", i);
			}
		}
	}
	return;
}

void get_sorted_vertexes(graph* x) {
	int* sorted_vertexes = (int*)malloc(x->n * sizeof(int));
	if (!topological_sort(x, sorted_vertexes)) {
		printf("Old: ");
		for (int i = 0; i < x->n; i++) printf("%d ", i);
		printf("\nNew: ");
		for (int i = 0; i < x->n; i++) printf("%d ", sorted_vertexes[i]);
	}
	return;
}

void get_scc(graph* g) {
	int* parts = (int*)malloc(g->n * sizeof(int));
	if (parts) {
		for (int i = 0; i < g->n; i++) parts[i] = -1;

		if (tarjan_scc(g, parts)) {
			printf("ERROR: Unable to get graph SCC's");
			return;
		}
		else {
			printf("Vertexes: ");
			for (int i = 0; i < g->n; i++) printf("%d ", i);
			printf("\nSCCs:     ");
			for (int i = 0; i < g->n; i++) printf("%d ", parts[i]);
		}
	}
	return;
}

void scc_test() {
	int n = 3;
	graph* x = graph_init(n);
	add_arc(x, 0, 1);
	add_arc(x, 1, 0);
	add_arc(x, 0, 2);
	add_arc(x, 2, 0);
	graph_print(x);
	get_scc(x);
}

void sat_solver_test() {
	char* test = "(x1 V ~x2)(x2 V x3)(~x2 V ~x3)(~x1 V x3)";
	sat_node* expr = get_expression(test);
	if (sat_solver(expr)) printf("ERROR");
}

int main() {
	sat_solver_test();
	return 0;
}
