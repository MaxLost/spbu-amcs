#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <iostream>

struct node {
    int num;
    int cond;
    node* next;
};

typedef struct node node;

node* create_node(int num, int cond, node* next = NULL);

//void stack_push(node** head, int val);
//
//int stack_pop(node** head);
//
//void queue_push(node** head, int val);
//
//int queue_pop(node** head);

void list_free(node** head);

#endif // LIST_H_INCLUDED