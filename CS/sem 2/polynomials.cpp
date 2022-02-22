#include <iostream>
#include <cstring>

struct poly{
    int k;
    int deg;
    struct poly *next;
};

typedef struct poly poly;

void poly_free(poly *p){
    if (p == NULL)
        return;
    poly_free(p->next);
    free(p);
    return;
}

void poly_insert(poly **m, poly *a){
    poly *p = a;
    while (p != NULL){
        poly *iter = *m;
        poly *prev = NULL;
        while (iter != NULL){
            if (iter->k == 0 && iter->deg == 0){
                iter->k = p->k;
                iter->deg = p->deg;
                break;
            }
            if (iter->deg < p->deg){
                poly *t = (poly *) malloc(sizeof(poly));
                t->k = p->k;
                t->deg = p->deg;
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
            else if (iter->deg == p->deg){
                iter->k += p->k;
                break;
            }
            prev = iter;
            iter = iter->next;
        }
        if (iter == NULL){
            poly *t = (poly *) malloc(sizeof(poly));
            t->k = p->k;
            t->deg = p->deg;
            t->next = NULL;
            prev->next = t;
        }
        p = p->next;
    }
    return;
}

poly *poly_get(const char *str){
    if (str == NULL){
        printf("Incorrect input");
        return NULL;
    }

    poly *result = (poly *) malloc(sizeof(poly));
    result->k = 0; result->deg = 0; result->next = NULL;

    poly *p = (poly *) malloc(sizeof(poly));
    p->k = 0; p->deg = 0; p->next = NULL;

    poly *iter = result;

    int sign = 1, buffer = 0;
    char *s = (char *) str;
    while (*s != '\0'){
        if (*s == '-'){
            sign = -1;
            ++s;
        }
        else if (*s >= '0' && *s <= '9'){
            buffer = strtol(s, &s, 10);
            p->k = buffer * sign;
            if (*s != 'x'){
                poly_insert(&result, p);
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
            poly_insert(&result, p);
            p->k = 0; p->deg = 0;
        }
        else if (*s =='^' || *s == '+')
            ++s;
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
    *result = poly_get("0");
    poly *i = (poly *) a;
    poly *prev = *result;
    while (i != NULL){
        poly *t = (poly *) malloc(sizeof(poly));
        t->k = i->k;
        t->deg = i->deg;
        t->next = NULL;
        if (prev->k == 0 && prev->deg == 0){
            prev->k = t->k;
            prev->deg = t->deg;
        }
        else {
            prev->next = t;
            prev = t;
        }
        i = i->next;
    }
    return;
}

void poly_add(poly *a, poly *b, poly **result){
    poly_clone(result, a);
    poly_insert(result, b);
    return;
}

void poly_multiply(const poly *a, const poly *b, poly **result){
    poly_free(*result);
    *result = poly_get("0");
    poly *i = (poly *) a;
    while (i != NULL){
        poly *j = (poly *) b;
        if (i->k == 0){
            break;
        }
        while (j != NULL){
            if (j->k == 0){
                break;
            }
            poly *t = (poly *) malloc (sizeof(poly));
            t->k = i->k * j->k;
            t->deg = i->deg + j->deg;
            t->next = NULL;
            poly_insert(result, t);
            j = j->next;
        }
        i = i->next;
    }
    return;
}

char *get_string_input(){
    char *input = (char *) malloc(64);
    int input_size = 0;
    char c;
    while ((c = getchar()) != '\n'){
        if ((input_size + 1) & 63){
            input = (char *) realloc(input, ((input_size + 1) / 64) + 64);
        }
        input[input_size] = c;
        ++input_size;
    }
    input = (char *) realloc(input, input_size + 1);
    input[input_size] = '\0';
    if (input)
        return input;
    return NULL;
}

int main(){
    char *input = get_string_input();
    poly *p = poly_get(input);
    poly *q = NULL;
    poly_clone(&q, p);
    poly *t = poly_get("0");
    poly_multiply(q, p, &t);
    poly_print(t);
    poly_free(p); poly_free(q); poly_free(t);
    return 0;
}
