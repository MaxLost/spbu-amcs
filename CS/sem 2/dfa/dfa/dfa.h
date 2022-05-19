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
int dfa_check(dfa* a, int x);
void dfa_print(dfa* a, int x);

int dfa_set_transition(dfa* a, int origin, int destination, int condition);
int dfa_reset_transition(dfa* a, int origin, int destination, int condition);
int dfa_set_final_state(dfa* a, int n);

dfa* dfa_get_number_recognizer(int x);

dfa* dfa_complement(dfa* a);
dfa* dfa_intersection(dfa* a, dfa* b);
dfa* dfa_union(dfa* a, dfa* b);
dfa* dfa_difference(dfa* a, dfa* b);

#endif // DFA_H_INCLUDED