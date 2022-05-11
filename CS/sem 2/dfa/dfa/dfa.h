#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

#include "graph.h"

struct dfa {
	graph *g;
	int final_num;
	int* final;
	int failure;
};

typedef struct dfa dfa;

dfa* dfa_init(int n);

void dfa_free(dfa* ptr);

int dfa_add_transition(dfa* a, int origin, int destination, int condition);
int dfa_delete_transition(dfa* a, int origin, int destination, int condition);

int dfa_set_final_state(dfa* a, int n);

int dfa_check(dfa* a, int x);

dfa* dfa_get_number_recognizer(int x);

void dfa_print(dfa* a, int x);

#endif // DFA_H_INCLUDED