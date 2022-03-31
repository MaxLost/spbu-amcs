#ifndef POLYNOMIALS_H_INCLUDED
#define POLYNOMIALS_H_INCLUDED

#include "polynomials.cpp"

struct poly{
    int k;
    int deg;
    struct poly *next;
};

typedef struct poly poly;

void poly_free(poly *p); // Free memory on pointer P

void poly_insert(poly **m, poly *a); // Add polynomial A to polynomial M

poly *poly_get(const char *str); // Create polynomial from string. Returns pointer on polynomial

void poly_print(const poly *p); // Print polynomial

void poly_clone(poly **result, const poly *a); // Create a copy of polynomial A on pointer RESULT

void poly_add(poly *a, poly *b, poly **result); // Summarize polynomials A and B on pointer RESULT

void poly_multiply(const poly *a, const poly *b, poly **result); // Multiply polynomials A and B on pointer RESULT

char *get_string_input(); // Get string input from stdin. Stops on '\n' symbol

#endif // POLYNOMIALS_H_INCLUDED
