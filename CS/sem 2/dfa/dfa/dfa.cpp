#include "dfa.h"

dfa* dfa_init(int n) {
	dfa* result = (dfa*)malloc(sizeof(dfa));
	if (result) {
		graph* g = graph_init(n + 1);
		result->g = g;
		result->failure = n;
		result->final_num = 0;
		result->final = NULL;
		for (int i = 0; i < n; i++) {
			add_arc(result->g, i, result->failure, 0);
			add_arc(result->g, i, result->failure, 1);
		}
	}
	else {
		return NULL;
	}
	return result;
}

void dfa_free(dfa* ptr) {
	graph_free(ptr->g);
	free(ptr->final);
	free(ptr);
	return;
}

int dfa_add_transition(dfa* a, int origin, int destination, int condition) {
	//adj_node* adj_ptr = find_adj_node(origin, a->g);
	node* arc_ptr = find_arc(a->g, origin, condition);
	if (arc_ptr) {
		arc_ptr->num = destination;
	}
	else {
		add_arc(a->g, origin, destination, condition);
	}
	return 0;
}

int dfa_delete_transition(dfa* a, int origin, int destination, int condition) {
	adj_node* adj_ptr = find_adj_node(origin, a->g);
	node* arc_ptr = find_arc(a->g, origin, destination, condition);
	if (arc_ptr) {
		arc_ptr->num = a->failure;
	}
	return 0;
}

int dfa_set_final_state(dfa* a, int n) {
	int* t = (int*)malloc(((a->final_num) + 1) * sizeof(int));
	if (!t) return 1;
	memcpy((void*) t, (void*) a->final, a->final_num * sizeof(int));
	++(a->final_num);
	free(a->final);
	a->final = t;
	a->final[a->final_num - 1] = n;
	return 0;
}

int dfa_next_state(dfa* a, int current_state, int x) {
	node* ptr = find_adj_node(current_state, a->g)->arc_list;
	while (ptr) {
		if (ptr->cond == x)
			return ptr->num;
		ptr = ptr->next;
	}
	return a->failure;
}

int dfa_check(dfa* a, int x) {
	int current_state = 0;
	while (x > 0 || (!x && !current_state)) {
		int d = x & 1;
		current_state = dfa_next_state(a, current_state, d);
		if (current_state == a->failure) {
			return 0;
		}
		x >>= 1;
	}
	for (int i = 0; i < a->final_num; i++) {
		if (current_state == a->final[i]) return 1;
	}
	return 0;
}

dfa *dfa_get_number_recognizer(int x) {
	int k = 0;
	while (x >= (1 << k)) k++;
	
	dfa* result = dfa_init(k + 1);
	for (int i = 0; i <= k; i++) {
		int d = x & 1;
		x >>= 1;
		dfa_add_transition(result, i, i + 1, d);
	}
	dfa_add_transition(result, k, k, 0);
	dfa_set_final_state(result, k);
	return result;
}

void dfa_print(dfa* a, int x) {
	for (int i = 0; i <= x; i++) {
		if (dfa_check(a, i)) printf("%d ", i);
	}
	return;
}