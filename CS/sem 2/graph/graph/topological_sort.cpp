#include "topological_sort.h"

int check_neighbours(int v, adj_node *adj_ptr, int* status) {
	while (adj_ptr->num != v) {
		adj_ptr = adj_ptr->next;
	}
	node* arc_ptr = adj_ptr->arc_list;
	while (arc_ptr) {
		if (status[arc_ptr->num] == 1) {
			return 1;
		}
		arc_ptr = arc_ptr->next;
	}
	return 0;
}

int topological_sort(graph* g, int* sorted_vertexes) {
	int* status = (int *) calloc(g->n, sizeof(int));
	int out_num = g->n - 1;
	for (int i = 0; i < g->n; i++) {
		if (!status[i]) {
			node* stack = create_node(i);
			while (stack) {
				int v = stack->num;
				if (status[v] == 2) {
					stack_pop(&stack);
				}
				else {
					adj_node* adj_ptr = g->adj_list;
					while (adj_ptr->num != v) {
						adj_ptr = adj_ptr->next;
					}
					node* arc_ptr = adj_ptr->arc_list;

					if (!arc_ptr || !check_neighbours(arc_ptr->num, g->adj_list, status)) {
						status[adj_ptr->num] = 2;
						sorted_vertexes[out_num] = adj_ptr->num;
						out_num--;
					}

					while (arc_ptr) {
						switch (status[arc_ptr->num]) {
						case 0: {
							stack_push(&stack, arc_ptr->num);
							break;
						}
						case 1: {
							printf("ERROR: Graph has cycles");
							return 1;
						}
						case 2: {
							break;
						}
						default: printf("ERROR: Undefined error"); return 1;
						}
						arc_ptr = arc_ptr->next;
					}
				}
			}
		}
	}
	return 0;
}
