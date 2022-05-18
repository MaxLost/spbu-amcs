#include "dfa.h"

dfa* dfa_init(int n) {
	dfa* result = (dfa*)malloc(sizeof(dfa));
	if (result) {
		graph* g = graph_init(n + 1);
		result->g = g;
		result->failure = n;
		result->final_num = 0;
		result->final = NULL;
		for (int i = 0; i <= n; i++) {
			add_arc(result->g, i, result->failure, 0);
			add_arc(result->g, i, result->failure, 1);
		}
	}
	else {
		return NULL;
	}
	return result;
}

dfa *dfa_clone(dfa* src) {
	dfa* result = dfa_init(src->g->n - 1);
	result->final_num = src->final_num;
	result->final = (int*)malloc(result->final_num * sizeof(int));
	if (!result->final) {
		dfa_free(result);
		return NULL;
	}
	memcpy((void*)result->final, (void*)src->final, src->final_num * sizeof(int));
	adj_node* adj_ptr = src->g->adj_list;
	while (adj_ptr) {
		node* arc_ptr = adj_ptr->arc_list;
		while (arc_ptr) {
			dfa_add_transition(result, adj_ptr->num, arc_ptr->num, arc_ptr->cond);
			arc_ptr = arc_ptr->next;
		}
		adj_ptr = adj_ptr->next;
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
	node* arc_ptr = find_arc(a->g, origin, condition);
	if (arc_ptr && arc_ptr->num == a->failure) {
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

int dfa_delete_all_transitions(dfa* a, int origin) {
	adj_node* adj_ptr = find_adj_node(origin, a->g);
	list_free(&(adj_ptr->arc_list));
	dfa_add_transition(a, adj_ptr->num, a->failure, 0);
	dfa_add_transition(a, adj_ptr->num, a->failure, 1);
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
	int counter = 0;
	while (x > 0 || (!x && !current_state)) {
		int d = x & 1;
		current_state = dfa_next_state(a, current_state, d);
		if (current_state == a->failure) {
			break;
		}
		x >>= 1;
		++counter;
		if (counter > 1000)
			break;
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

int dfa_cartesian_minimize(dfa** x, int an, int bn) {
	adj_node* adj_ptr = (*x)->g->adj_list;

	while (adj_ptr) {
		node* arc_ptr = adj_ptr->arc_list;
		int* t = (int *) malloc(2 * sizeof(int));
		if (!t) return 1;
		int c[2] = {0, 0};
		while (arc_ptr) {
			if (arc_ptr->num == (*x)->failure) {
				t[arc_ptr->cond] = (*x)->failure;
				c[arc_ptr->cond] = 9;
			}
			else if (!c[arc_ptr->cond]) {
				c[arc_ptr->cond]++;
				t[arc_ptr->cond] = arc_ptr->num % bn;
			}
			else if (c[arc_ptr->cond] == 1) {
				c[arc_ptr->cond]++;
				t[arc_ptr->cond] += (arc_ptr->num / bn) * bn;
			}
			arc_ptr = arc_ptr->next;
		}
		dfa_delete_all_transitions((*x), adj_ptr->num);
		dfa_add_transition((*x), adj_ptr->num, t[0], 0);
		dfa_add_transition((*x), adj_ptr->num, t[1], 1);
		adj_ptr = adj_ptr->next;
	}
	return 0;
}

dfa* dfa_cartesian_product(dfa* a, dfa* b) {
	int an = a->g->n, bn = b->g->n;
	
	dfa* result = dfa_init((an * bn));
	for (int i = 0; i < an; i++) {
		int m = i * bn;
		for (int j = 0; j < bn; j++) {
			node *t = find_arc(b->g, j, 0);
			if (t) dfa_add_transition(result, m + j, m + t->num, 0);
			
			t = find_arc(b->g, j, 1);
			if (t) dfa_add_transition(result, m + j, m + t->num, 1);
			
			t = find_arc(a->g, i, 0);
			if (t) dfa_add_transition(result, m + j, (t->num) * bn + j, 0);
			
			t = find_arc(a->g, i, 1);
			if (t) dfa_add_transition(result, m + j, (t->num) * bn + j, 1);
		}
	}
	graph_print(result->g);
	int t = dfa_cartesian_minimize(&result, an, bn);
	if (t) {
		dfa_free(result);
		return NULL;
	}

	return result;
}

dfa* dfa_complement(dfa* a) {
	dfa* comp = dfa_clone(a);
	int new_final_num = comp->g->n - comp->final_num;
	int* new_final = (int*)malloc((new_final_num) * sizeof(int));
	if (!new_final || !comp) {
		dfa_free(comp);
		return NULL;
	}
	int k = 0;
	for (int i = 0; i <= comp->g->n; i++) {
		int is_final = 0;
		for (int j = 0; j < comp->final_num; j++) {
			if (comp->final[j] == i) {
				is_final = 1;
				break;
			}
		}
		if (!is_final) {
			new_final[k] = i;
			++k;
		}
	}
	comp->final_num = new_final_num;
	free(comp->final);
	comp->final = new_final;
	return comp;
}

dfa* dfa_intersection(dfa* a, dfa* b) {
	if (a->g->n < b->g->n) {
		dfa* c = a;
		a = b;
		b = c;
	}
	dfa* result = dfa_cartesian_product(a, b);
	for (int i = 0; i < a->final_num; i++) {
		for (int j = 0; j < b->final_num; j++) {
			dfa_set_final_state(result, a->final[i] * (b->g->n - 1) + b->final[j]);
		}
	}
	return result;
}

dfa* dfa_union(dfa* a, dfa* b) {
	if (a->g->n < b->g->n) {
		dfa* c = a;
		a = b;
		b = c;
	}
	dfa* result = dfa_cartesian_product(a, b);
	for (int i = 0; i < a->final_num; i++) {
		for (int j = 0; j < b->g->n; j++) {
			dfa_set_final_state(result, a->final[i] * b->g->n + j);
		}
	}
	for (int i = 0; i < b->final_num; i++) {
		for (int j = 0; j < a->g->n; j++) {
			dfa_set_final_state(result, j * b->g->n + b->final[i]);
		}
	}
	dfa_set_final_state(result, result->failure);
	return result;
}

dfa* dfa_difference(dfa* a, dfa* b) {
	dfa* result = dfa_intersection(a, dfa_complement(b));
	if (!result) return NULL;
	return result;
}