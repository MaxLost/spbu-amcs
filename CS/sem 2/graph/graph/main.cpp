#include <stdio.h>
#include "graph.h"
#include "bipartity_check.h"
#include "topological_sort.h"
#include "file_input.h"
#include "scc_partition.h"

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

		if (scc_partition(g, parts)) {
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

int main() {
	int n = 4;
	/*
	printf("Enter number of vertexes of graph: ");
	scanf_s("%d", &n);
	getchar();
	*/
	/*graph* x = graph_init(n);
	add_arc(x, 0, 1);
	add_arc(x, 0, 2);
	add_arc(x, 1, 0);
	add_arc(x, 2, 3);
	add_arc(x, 3, 2);*/
	const char* source_path = CUR_DIR "test_graph.txt";
	graph* x = graph_read(source_path);
	graph_print(x);
	get_scc(x);
	const char* out_path = CUR_DIR "out_graph.txt";
	graph_write(x, out_path);
	/*const char* source_path = CUR_DIR "test_graph.txt";
	graph* g = graph_read(source_path);
	graph_print(g);
	const char* out_path = CUR_DIR "out_graph.txt";
	graph_write(g, out_path);*/
	
	/*int* a = (int*)calloc(n, sizeof(int));
	int* b = (int*)calloc(n, sizeof(int));
	int k = 0, l = 0;
	if (bipartity_check_bfs(x, &a, &k, &b, &l)) {
		printf("Graph is not biparted");
	}
	else {
		printf("1st part: ");
		for (int i = 0; i < k; i++) {
			printf("%d ", a[i]);
		}
		printf("\n2nd part: ");
		for (int i = 0; i < l; i++) {
			printf("%d ", b[i]);
		}
	}*/
	return 0;
}
