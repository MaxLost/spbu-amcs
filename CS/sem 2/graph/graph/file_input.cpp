#include "file_input.h"

graph* graph_read(const char* path) {
	FILE* file = fopen(path, "r");
	if (file) {
		int n = 0;
		if (fscanf(file, "%d", &n) == EOF) {
			printf("ERROR: Unable to read graph");
			return NULL;
		}
		graph* result = graph_init(n);
		char* str = (char*)malloc(n * 4);
		for (int i = 0; i < n; i++) {
			int origin;
			if (fscanf(file, "%d", &origin)) {
				int c = fgetc(file);
				while (c != (int) '\n' && c != EOF) {
					int buffer;
					if (fscanf(file, "%d", &buffer)) {
						add_arc(result, origin, buffer);
						c = fgetc(file);
						if (c != (int) '\n' && c != (int) ',' && c != EOF && (c < (int)'0' || c >(int) '9')) {
							graph_free(result);
							printf("ERROR: Unexpected symbol");
							return NULL;
						}
					}
					else {
						graph_free(result);
						printf("ERROR: Unexpected symbol");
						return NULL;
					}
				}
			}
		}
		fclose(file);
		return result;
	}
	else {
		printf("ERROR: Unable to open file");
	}
	return NULL;
}

int graph_write(graph* g, const char* path) {
	FILE* file = fopen(path, "w");
	if (file) {
		fprintf(file, "%d\n", g->n);
		adj_node* adj_ptr = g->adj_list;
		while(adj_ptr) {
			fprintf(file, "%d,", adj_ptr->num);
			node* arc_ptr = adj_ptr->arc_list;
			if (arc_ptr) {
				while (arc_ptr->next) {
					fprintf(file, "%d,", arc_ptr->num);
					arc_ptr = arc_ptr->next;
				}
				fprintf(file, "%d", arc_ptr->num);
			}
			fprintf(file, "\n");
			adj_ptr = adj_ptr->next;
		}
		fclose(file);
		return 0;
	}
	return 1;
}