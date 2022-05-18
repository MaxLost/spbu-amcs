﻿#include "dfa.h"

void dfa_check1() {
	dfa* a = dfa_init(5);
	int num = 10;
	dfa_add_transition(a, 0, 1, 0);
	dfa_add_transition(a, 1, 2, 1);
	dfa_add_transition(a, 2, 3, 0);
	dfa_add_transition(a, 3, 4, 1);
	dfa_set_final_state(a, 4);
	if (dfa_check(a, num)) {
		printf("This DFA recognizes %d", num);
	}
	else {
		printf("This DFA didn't recognize number %d", num);
	}
	return;
}

void dfa_check2(int n) {
	dfa* a = dfa_get_number_recognizer(n);
	if (dfa_check(a, n)) {
		printf("This DFA recognizes %d\n", n);
	}
	else {
		printf("This DFA didn't recognize number %d\n", n);
	}
	dfa* b = dfa_complement(a);
	graph_print(b->g);
	printf("%d\n", b->g->n);
	for (int i = 0; i < b->final_num; i++) {
		printf("%d ", b->final[i]);
	}
	printf("\n");
	dfa_print(b, n);
	return;
}

void dfa_check3() {
	dfa* a = dfa_init(4);
	dfa_add_transition(a, 0, 1, 0);
	dfa_add_transition(a, 0, 2, 1);
	dfa_add_transition(a, 1, 3, 1);
	dfa_add_transition(a, 2, 3, 0);
	dfa_add_transition(a, 3, 1, 0);
	dfa_add_transition(a, 3, 2, 1);
	dfa_set_final_state(a, 2);
	dfa_set_final_state(a, 3);
	dfa_print(a, 32);
	return;
}

void dfa_check4() {
	dfa* a = dfa_get_number_recognizer(5);
	dfa* b = dfa_get_number_recognizer(3);
	dfa* c = dfa_union(a, b);
	graph_print(c->g);
	dfa_print(c, 5);
	return;
}

int main() {
	//dfa_check1();
	/*int n;
	printf("Enter number: ");
	scanf("%d", &n);
	dfa_check2(n);*/
	//dfa_check3();
	dfa_check4();
	return 0;
}