#include "2_sat_solver.h"

int is_var_in_expr(node* list, int var) {
	while (list) {
		if (list->num == var || list->num == -var) return 1;
		list = list->next;
	}
	return 0;
}

int count_variables(sat_node* list) {
	if (!list) return -1;
	node* v = create_node(list->l);
	if (!is_var_in_expr(v, list->r)) stack_push(&v, list->r);
	list = list->next;
	while (list) {
		if (!is_var_in_expr(v, list->l)) stack_push(&v, list->l);
		if (!is_var_in_expr(v, list->r)) stack_push(&v, list->r);
		list = list->next;
	}
	int c = 0;
	node* ptr = v;
	while (ptr) {
		++c;
		ptr = ptr->next;
	}
	free(v);
	return c;
}

int get_vertex_num(int n) {
	int result;
	int sign = (n > 0) ? 1 : 0;
	if (n == 1) result = 0;
	else if (n == -1) result = 1;
	else {
		if (sign) result = 2 * (n - 1);
		else result = 2 * (abs(n) - 1) + 1;
	}
	return result;
}

sat_node* sat_node_create(int l = 0, int r = 0) {
	sat_node* result = (sat_node*)malloc(sizeof(sat_node));
	result->l = l;
	result->r = r;
	result->next = NULL;
	return result;
}

void sat_node_free(sat_node* ptr) {
	while (ptr) {
		sat_node* t = ptr;
		ptr = ptr->next;
		free(t);
	}
	return;
}

sat_node* get_expression(char *str) {
	sat_node* result = sat_node_create();
	sat_node* expr_ptr = result;
	sat_node* p = sat_node_create();
	int k = 1, opened = 0, closed = 0;
	while (*str != '\0') {
		switch (*str) {
			case 'x': {
				++str;
				k *= strtol(str, &str, 10);
				if (p->l == 0) {
					p->l = k;
				}
				else {
					p->r = k;
				}
				k = 1;
				break;
			}
			case '~': {
				k = -1;
				++str;
				break;
			}
			case '(': {
				++str;
				++opened;
				break;
			}
			case ')': {
				++str;
				++closed;
				break;
			}
			case 'V':
			case ' ': {
				++str;
				break;
			}
			default: {
				printf("ERROR: Unexpected symbol");
				sat_node_free(p);
				sat_node_free(result);
				return result;
			}
		}
		if (p->l != 0 && p->r != 0) {
			if (expr_ptr == result && expr_ptr->l == 0) {
				expr_ptr->l = p->l;
				expr_ptr->r = p->r;
			}
			else {
				sat_node *t = sat_node_create(p->l, p->r);
				expr_ptr->next = t;
				expr_ptr = t;
			}
			p->l = 0; p->r = 0;
		}
	}
	if (opened != closed) {
		printf("ERROR: Wrong expression\n");
		sat_node_free(p);
		sat_node_free(result);
		return result;
	}
	sat_node_free(p);
	return result;
}

int sat_solver(sat_node *expr) {
	if (!expr) {
		return 1;
	}
	int c = count_variables(expr);
	if (c == -1) {
		printf("ERROR: Unable to count variables");
		return 1;
	}
	graph* g = graph_init(c * 2);
	while (expr) {
		int l1 = expr->l * (-1);
		l1 = get_vertex_num(l1);
		int r1 = expr->r * 1;
		r1 = get_vertex_num(r1);
		add_arc(g, l1, r1);
		int l2 = expr->r * (-1);
		l2 = get_vertex_num(l2);
		int r2 = expr->l * 1;
		get_vertex_num(r2);
		add_arc(g, l2, r2);
		expr = expr->next;
	}
	int* parts = (int*)malloc(g->n * sizeof(int));
	if (!parts) {
		return 1;
	}
	for (int i = 0; i < g->n; i++) parts[i] = -1;

	tarjan_scc(g, parts);
	int max = 1;
	for (int i = 0; i < g->n; i += 2) {
		if (parts[i] == parts[i + 1]) {
			printf("ERROR: No solutions\n");
			return 0;
		}
	}
	int* solution = (int*)malloc(g->n * sizeof(int));
	if (!solution) return 1;
	for (int i = 0; i < g->n; i++) solution[i] = -1;
	for (int i = 0; i < g->n; i += 2) {
		if (parts[i] > parts[i + 1]) {
			solution[i] = 1;
			solution[i + 1] = 0;
		}
		else {
			solution[i] = 0;
			solution[i + 1] = 1;
		}
	}
	for (int i = 0; i < g->n; i += 2) {
		printf("x%d: %d\n", (i / 2) + 1, solution[i]);
	}
	return 0;
}