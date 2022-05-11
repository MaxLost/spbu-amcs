#include "dfa.h"

void dfa_check1() {
	dfa* a = dfa_init(5);
	int num = 10;
	dfa_add_transition(a, 0, 1, 0);
	dfa_add_transition(a, 1, 2, 1);
	dfa_add_transition(a, 2, 3, 0);
	dfa_add_transition(a, 3, 4, 1);
	//graph_print(a->g);
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
	graph_print(a->g);
	if (dfa_check(a, n)) {
		printf("This DFA recognizes %d", n);
	}
	else {
		printf("This DFA didn't recognize number %d", n);
	}
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
	dfa_print(a, 9);
	return;
}

int main() {
	//dfa_check1();
	/*int n;
	printf("Enter number: ");
	scanf("%d", &n);
	dfa_check2(n);*/
	//dfa_check3();
	return 0;
}