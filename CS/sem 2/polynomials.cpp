#include <iostream>
#include <cstring>
#include "str_input.h"

struct poly{
    int k;
    int deg;
    struct poly *next;
};

typedef struct poly poly;

poly *poly_get_monomial(int k, int deg){
    poly *t = (poly *) malloc(sizeof(poly));
    t->k = k;
    t->deg = deg;
    t->next = NULL;
    return t;
}

void poly_free(poly *p){
    poly *t;
    while (p != NULL){
        t = p->next;
        free(p);
        p = t;
    }
    return;
}

void poly_merge(poly **m, poly *a){
    int flag = 0;
    while (a){
        poly *iter = *m;
        poly *prev = NULL;
        while (iter){
            flag = 0;
            if (iter->k == 0 && iter->deg == 0){
                iter->k = a->k;
                iter->deg = a->deg;
                break;
            }
            if (iter->deg < a->deg){
                poly *t = poly_get_monomial(a->k, a->deg);
                if (iter == *m){
                    t->next = *m;
                    *m = t;
                }
                else {
                    prev->next = t;
                    t->next = iter;
                }
                break;
            }
            else if (iter->deg == a->deg){
                iter->k += a->k;
                if (iter->k == 0){
                    iter->deg = 0;
                    if (prev){
                        prev->next = iter->next;
                    }
                    else if (iter->next){
                        poly *t = iter;
                        iter = iter->next;
                        free(t);
                        prev = iter;
                    }
                    flag = 1;
                }
                break;
            }
            prev = iter;
            iter = iter->next;
        }
        if (!iter && prev && !flag){
            poly *t = poly_get_monomial(a->k, a->deg);
            prev->next = t;
        }
        a = a->next;
    }
    return;
}

poly *poly_get(const char *str){
    if (str == NULL){
        printf("Incorrect input");
        return NULL;
    }

    poly *result = poly_get_monomial(0, 0);
    poly *p = poly_get_monomial(0, 0);

    poly *iter = result;

    int sign = 1, buffer = 0;
    char *s = (char *) str;
    while (*s != '\0'){
        if (*s == '-'){
            sign *= -1;
            ++s;
        }
        else if (*s >= '0' && *s <= '9'){
            buffer = strtol(s, &s, 10);
            p->k = buffer * sign;
            if (*s != 'x'){
                poly_merge(&result, p);
                p->k = 0; p->deg = 0;
            }
            sign = 1;
        }
        else if (*s == 'x'){
            if (*(s - 1) < '0' || *(s - 1) > '9')
                p->k = 1 * sign;
            if (*(s + 1) != '\0' && *(s + 1) != '^'){
                p->deg = 1;
                ++s;
            }
            else if (*(s + 1) == '\0'){
                p->deg = 1;
                ++s;
            }
            else {
                buffer = strtol(s + 2, &s, 10);
                if (buffer)
                    p->deg = buffer;
                else
                    ++s;
            }
            poly_merge(&result, p);
            p->k = 0; p->deg = 0;
            sign = 1;
        }
        else if (*s == '+'){
            ++s;
            if (*s == 'x'){
                p->k = 1;
            }
        }
        else {
            printf("Incorrect input");
            return NULL;
        }

    }
    return result;
}

void poly_print(const poly *p){
    if (p != NULL){
        if (p->k != 1 && p->k != -1 && p->deg > 0){
            printf("%d", p->k);
        }
        else if (p->k == -1 && p->deg > 0){
            printf("-");
        }
        else if (p->deg == 0){
            printf("%d", p->k);
        }
        if (p->deg > 1)
            printf("x^%d", p->deg);
        else if (p->deg == 1)
            printf("x");
        p = p->next;
    }
    while (p != NULL){
        if (p->k > 0){
            printf("+");
        }
        if (p->k != 1 && p->k != -1 && p->deg > 0){
            printf("%d", p->k);
        }
        else if (p->k == -1 && p->deg > 0){
            printf("-");
        }
        else if (p->deg == 0){
            printf("%d", p->k);
        }
        if (p->deg > 1)
            printf("x^%d", p->deg);
        else if (p->deg == 1)
            printf("x");
        p = p->next;
    }
    printf("\n");
}

void poly_clone(poly **result, const poly *a){
    poly_free(*result);
    *result = poly_get_monomial(0, 0);
    poly *prev = *result;
    poly *t = NULL;
    while (a){
        if (prev->k == 0 && prev->deg == 0){
            t = prev;
        }
        else {
            t = (poly *) malloc(sizeof(poly));
        }
        memcpy((void *) t, (void *) a, sizeof(poly));
        t->next = NULL;
        prev->next = t;
        prev = t;
        a = a->next;
    }
    prev->next = NULL;
    return;
}

void poly_add(poly *a, poly *b, poly **result){
    poly_clone(result, a);
    poly_merge(result, b);
    return;
}

void poly_multiply(const poly *a, const poly *b, poly **result){
    poly_free(*result);
    *result = poly_get_monomial(0, 0);
    poly *i = (poly *) a;
    while (i){
        poly *j = (poly *) b;
        if (i->k == 0){
            break;
        }
        while (j){
            if (j->k == 0){
                break;
            }
            poly *t = poly_get_monomial((i->k) * (j->k), (i->deg) + (j->deg));
            poly_merge(result, t);
            j = j->next;
        }
        i = i->next;
    }
    return;
}

int main(){
    char *input = get_string_input();
    poly *summ = poly_get(input);

    input = get_string_input();
    poly *q = poly_get(input);

    poly *t = poly_get_monomial(0, 0);
    poly_multiply(summ, q, &t);
    poly_print(t);
    /*while (true){
        printf("Enter polynomial or \"stop\": ");
        char *input = get_string_input();
        if (!strcmp(input, "stop")){
            printf("Final summ: ");
            poly_print(summ);
            break;
        }
        poly *p = poly_get(input);
        poly_merge(&summ, p);
        printf("Current summ: ");
        poly_print(summ);
        printf("\n");
    }*/
    return 0;
}
