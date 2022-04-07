#include "bipartity_check.h"

int check_neighbours(adj_node* adj_ptr, int* color) {
    node* arc_ptr = adj_ptr->arc_list;
    while (arc_ptr){
        if (color[arc_ptr->num] == color[adj_ptr->num]) {
            return 1;
        }
        arc_ptr = arc_ptr->next;
    }
    return 0;
}

int bipart_graph(int* color, int n, int** a, int* k, int** b, int* l) {
    if (!(*k) || !(*l)) {
        *k = 0, * l = 0;
    }
    for (int i = 0; i < n; i++) {
        if (color[i] == 1) {
            (*a)[*k] = i;
            ++(*k);
        }
        else if (color[i] == 2) {
            (*b)[*l] = i;
            ++(*l);
        }
        else {
            printf("Error");
            return 1;
        }
    }
    return 0;
}

int inverse_color(int color) {
    if (color == 1) return 2;
    else return 1;
}

int is_bipartite(graph* g, int* color) {
    int start_color = 1;
    for (int i = 0; i < g->n; i++) {
        if (!color[i]) {
            node* stack = create_node(i);
            color[i] = start_color;
            start_color = inverse_color(start_color);
            while (stack) {
                int v = stack_pop(&stack);
                adj_node* adj_ptr = find_adj_node(v, g);
                if (check_neighbours(adj_ptr, color)) {
                    return 1;
                }
                else {
                    node* arc_ptr = adj_ptr->arc_list;
                    while (arc_ptr) { 
                        if (!color[arc_ptr->num]) {
                            stack_push(&stack, arc_ptr->num);
                            color[arc_ptr->num] = inverse_color(color[adj_ptr->num]);
                        }
                        arc_ptr = arc_ptr->next;
                    }
                }
            }
        }
    }
    return 0;
}

int bipartity_check_dfs(graph* g, int** a, int* k, int** b, int* l) {
    int* color = (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->n; i++) {
        if (!color[i]) {
            node* stack = create_node(i);
            color[i] = 1;
            while (stack) {
                int v = stack_pop(&stack);
                adj_node* adj_ptr = find_adj_node(v, g);
                if (check_neighbours(adj_ptr, color)) {
                    return 1;
                }
                else {
                    node* arc_ptr = adj_ptr->arc_list;
                    while (arc_ptr) {
                        if (!color[arc_ptr->num]) {
                            stack_push(&stack, arc_ptr->num);
                            color[arc_ptr->num] = inverse_color(color[adj_ptr->num]);
                        }
                        arc_ptr = arc_ptr->next;
                    }
                }
            }
        }
    }
    if (bipart_graph(color, g->n, a, k, b, l)) {
        return 1;
    }
    return 0;
}

int bipartity_check_bfs(graph* g, int** a, int* k, int** b, int* l) {
    int* color = (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->n; i++) {
        if (!color[i]) {
            node* queue = create_node(i);
            color[i] = 1;
            while (queue) {
                int v = queue_pop(&queue);
                adj_node* adj_ptr = find_adj_node(v, g);
                if (check_neighbours(adj_ptr, color)) {
                    return 1;
                }
                else {
                    node* arc_ptr = adj_ptr->arc_list;
                    while (arc_ptr) {
                        if (!color[arc_ptr->num]) {
                            queue_push(&queue, arc_ptr->num);
                            color[arc_ptr->num] = inverse_color(color[adj_ptr->num]);
                        }
                        arc_ptr = arc_ptr->next;
                    }
                }
            }
        }
    }
    if (bipart_graph(color, g->n, a, k, b, l)) {
        return 1;
    }
    return 0;
}
