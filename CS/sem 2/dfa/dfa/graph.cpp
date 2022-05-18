#include "graph.h"

adj_node *create_adj_node(int num, node *arc = NULL, adj_node *next = NULL){
    adj_node *result = (adj_node *) malloc(sizeof(adj_node));
    if (!result)
        return NULL;
    result->num = num;
    result->arc_list = arc;
    result->next = next;
    return result;
}

adj_node* find_adj_node(int v, graph *g) {
    adj_node* adj_ptr = g->adj_list;
    while (adj_ptr && adj_ptr->num != v) {
        adj_ptr = adj_ptr->next;
    }
    if (!adj_ptr) return NULL;
    return adj_ptr;
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

void add_arc(graph* g, int origin, int destination, int condition) {
    adj_node* i = find_adj_node(origin, g);
    node* ptr = i->arc_list;
    while (ptr && ptr->next) {
        if (ptr->num == destination) return;
        ptr = ptr->next;
    }
    node* t = create_node(destination, condition);
    if (!ptr) {
        i->arc_list = t;
    }
    else {
        ptr->next = t;
    }
    return;
}

node* find_arc(graph* g, int origin, int destination, int condition) {
    adj_node* adj_ptr = find_adj_node(origin, g);
    node* arc_ptr = adj_ptr->arc_list;
    while (arc_ptr && (arc_ptr->cond != condition || arc_ptr->num != destination)) {
        arc_ptr = arc_ptr->next;
    }
    if (!arc_ptr) return NULL;
    return arc_ptr;
}

node* find_arc(graph *g, int origin, int condition) {
    adj_node* adj_ptr = find_adj_node(origin, g);
    node* arc_ptr = adj_ptr->arc_list;
    while (arc_ptr && arc_ptr->cond != condition) {
        arc_ptr = arc_ptr->next;
    }
    if (!arc_ptr) return NULL;
    return arc_ptr;
}

void delete_arc(graph* g, int origin, int destination, int condition) {
    adj_node* i = find_adj_node(origin, g);
    node* ptr = i->arc_list, *prev = NULL;
    while (ptr && (ptr->num != destination || ptr->cond != condition)) {
        prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr) return;
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

void graph_print(graph* g) {
    adj_node *adj_ptr = g->adj_list;
    for (int i = 0; i < g->n; i++) {
        printf("%d: ", i);
        node* arc_ptr = adj_ptr->arc_list;
        while (arc_ptr) {
            printf("%d (%d) ", arc_ptr->num, arc_ptr->cond);
            arc_ptr = arc_ptr->next;
        }
        printf("\n");
        adj_ptr = adj_ptr->next;
    }
    return;
}
