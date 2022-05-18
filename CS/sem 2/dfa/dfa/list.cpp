#include "list.h"

node* create_node(int num, int cond, node *next) {
    node* result = (node*)malloc(sizeof(node));
    result->num = num;
	result->cond = cond;
    result->next = NULL;
    return result;
}

//void stack_push(node** head, int val) {
//	node* t = create_node(val, *head);
//	*head = t;
//	return;
//}
//
//int stack_pop(node** head) {
//	node* t = (*head)->next;
//	int result = (*head)->num;
//	free(*head);
//	*head = t;
//	return result;
//}
//
//void queue_push(node** head, int val) {
//	node* t = create_node(val);
//	node* ptr = *head;
//	if (!ptr) {
//		*head = t;
//		return;
//	}
//	while (ptr->next) {
//		ptr = ptr->next;
//	}
//	ptr->next = t;
//	return;
//}
//
//int queue_pop(node** head) {
//	node* ptr = *head;
//	*head = (*head)->next;
//	int val = ptr->num;
//	free(ptr);
//	return val;
//}

void list_free(node** head) {
	while (*head) {
		node* ptr = *head;
		*head = ptr->next;
		free(ptr);
	}
	return;
}
