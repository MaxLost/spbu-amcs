#include "graph.h"

adj_node *create_adj_node(int num, node *arc = NULL, adj_node *next = NULL){
    adj_node *result = (adj_node *) malloc(sizeof(adj_node));
    result->num = num;
    result->arc_list = arc;
    result->next = next;
    return result;
}

graph* graph_init(int n) {
    graph* result = (graph*)malloc(sizeof(graph));
    if (result) {
        result->n = n;
        adj_node* adj_list = create_adj_node(0);
        result->adj_list = adj_list;
        for (int i = 1; i < n; i++) {
            adj_node* t = create_adj_node(i);
            adj_list->next = t;
            adj_list = adj_list->next;
        }
    }
    return result;
}

graph *graph_get(int n){
    graph *result = (graph *) malloc(sizeof(graph));
    result->n = n;
    if (n > 1){
        adj_node *adj_list = create_adj_node(-1);
        result->adj_list = adj_list;

        printf("Enter end vertexes for arcs from current vertex: \n");

        for (int i = 0; i < n; i++){
            printf("%d: ", i);
            if (adj_list->num == -1){
                adj_list->num = i;
            }
            else {
                adj_node *t= create_adj_node(i);
                adj_list->next = t;
                adj_list = adj_list->next;
            }
            node *ptr = adj_list->arc_list;
            char *str = get_string_input();
            while(*str != '\0'){
                int v = strtol(str, &str, 10);
                if (v >= n){
                    printf("\nERROR: Incorrect vertex name");
                    return NULL;
                }
                node *t = create_node(v);
                if (!ptr){
                    adj_list->arc_list = t;
                }
                else{
                    ptr->next = t;
                    ptr = ptr->next;
                }
            }
        }
        return result;
    }
    else if (n == 1) {
        adj_node *adj_list = create_adj_node(0);
        result->adj_list = adj_list;
        return result;
    }
    else {
        return NULL;
    }
}

void graph_free(graph* g) {
    adj_node* adj_ptr = g->adj_list;
    while (adj_ptr) {
        node* arc_ptr = adj_ptr->arc_list;
        while (arc_ptr) {
            node* t = arc_ptr;
            arc_ptr = arc_ptr->next;
            free(t);
        }
        adj_node* t = adj_ptr;
        adj_ptr = adj_ptr->next;
        free(t);
    }
    free(g);
    return;
}

void add_arc(graph* g, int origin, int destination) {
    adj_node* i = g->adj_list;
    while (i->num != origin) {
        i = i->next;
    }
    node* ptr = i->arc_list;
    while (ptr && ptr->next) {
        if (ptr->num == destination) return;
        ptr = ptr->next;
    }
    node* t = create_node(destination);
    if (!ptr) {
        i->arc_list = t;
    }
    else {
        ptr->next = t;
    }
    return;
}

void add_edge(graph* g, int v, int w) {
    add_arc(g, v, w);
    add_arc(g, w, v);
    return;
}

void delete_arc(graph* g, int origin, int destination) {
    adj_node* i = g->adj_list;
    while (i->num != origin) {
        i = i->next;
    }
    node* ptr = i->arc_list, *prev = NULL;
    while (ptr->num != destination && ptr) {
        prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr) {
        printf("Edge from %d to %d does not exist", origin, destination);
    }
    else if (ptr == i->arc_list) {
        i->arc_list = ptr->next;
        free(ptr);
    }
    else {
        prev->next = ptr->next;
        free(ptr);
    }
    return;
}

void delete_edge(graph* g, int v, int w) {
    delete_arc(g, v, w);
    delete_arc(g, w, v);
    return;
}

void graph_print(graph* g) {
    adj_node *adj_ptr = g->adj_list;
    for (int i = 0; i < g->n; i++) {
        printf("%d: ", i);
        node* arc_ptr = adj_ptr->arc_list;
        while (arc_ptr) {
            printf("%d ", arc_ptr->num);
            arc_ptr = arc_ptr->next;
        }
        printf("\n");
        adj_ptr = adj_ptr->next;
    }
    return;
}

int check_neighbours(adj_node* ptr, int* color) {
    int v = ptr->num;
    node* p = ptr->arc_list;
    while (p) {
        if (color[p->num] == color[v]) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}
