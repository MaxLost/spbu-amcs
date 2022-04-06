#include <stdio.h>
#include "graph.h"
#include "bipartity_check.h"
#include "topological_sort.h"

void bipartite_test(graph* x) {

    int* color = (int*)calloc(x->n, sizeof(int));
    if (color) {
        //int* a = (int*)calloc(x->n, sizeof(int));
        //int* b = (int*)calloc(x->n, sizeof(int));
        //int k = 0, l = 0;

        if (is_bipartite(x, color)) {//bipartity_check_bfs(x, &a, &k, &b, &l)) {
            printf("Graph is not biparted");
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
}

void get_sorted_vertexes(graph* x) {
    int* sorted_vertexes = (int*)malloc(x->n * sizeof(int));
    if (topological_sort(x, sorted_vertexes)) {
        return;
    }
    else {
        for (int i = 0; i < x->n; i++) printf("%d ", i);
        printf("\n");
        for (int i = 0; i < x->n; i++) printf("%d ", sorted_vertexes[i]);
    }
}

int main() {
    int n = 4;
    /*
    printf("Enter number of vertexes of graph: ");
    scanf_s("%d", &n);
    getchar();
    */
    graph* x = graph_init(n);
    add_arc(x, 3, 0);
    add_arc(x, 0, 1);
    add_arc(x, 1, 2);
    //add_arc(x, 2, 1);
    graph_print(x);
    get_sorted_vertexes(x);
    //bipartite_test(x);
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
