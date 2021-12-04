#ifndef BIG_INT_H_INCLUDED
#define BIG_INT_H_INCLUDED

/*
 * Realization of data structure and functions for long arithmetics
 */

typedef struct big_int {
    unsigned int length;
    unsigned char sign; // 1 = '+', 0 = '-'
    unsigned char *number;
} big_int;

void big_int_print_bin(const big_int *n);

/* Constructor for big_int data structure / Takes binary representation of number in string / Returns pointer on big_int item */
big_int *big_int_get_bin(const char *bin_number);

/* Function for reset a big_int item / Takes pointer on big_int item */
void big_int_clear(big_int *n);

/* Function for assigning value of the second big_int item to first / Takes two pointers on big_int items, first is destination, second is source */
void big_int_assign(big_int *dest, const big_int *src);

/* Function for correct freeing memory from big_int item / Takes pointer on big_int item that you want to delete */
void big_int_free(big_int *n);

/* Function for optimizing memory allocated for big_int item / Takes pointer on big_int item that you want to optimize */
void big_int_resize(big_int *n);

/* Function for comparing 2 big_int values / Takes two pointers on big_int items and argument that defines type of comparison: normal or absolute.
 * Normal type of comparison includes comparison by signs of numbers. Absolute type compares numbers only by absolute values.
 * Returns an integer value from -1 to 1. Result meaning: 1 -> first > second, 0 -> first == second, -1 -> first < second.
 */
int big_int_compare(const big_int *n1, const big_int *n2, bool abs);

/* Function for adding big_int values / Takes two pointers on big_int items and pointer on big_int item in which result will be returned /
 * Returns big_int item - result of adding second value to first.
 */
void big_int_add(const big_int *n1, const big_int *n2, big_int *result);

/* Function for adding big_int values / Takes two pointers on big_int items / Result of adding second value to first will be returned in the first argument */
void big_int_add_direct(big_int *n1, const big_int *n2);

/* Function for subtracting one big_int value from another / Takes two pointes on big_int items and and pointer on big_int item in which result will be returned /
 * Returns big_int item - result of subtracting second value from first.
 */
void big_int_sub(const big_int *n1, const big_int *n2, big_int *result);


void big_int_left_shift(const big_int *n, int step, big_int *dst); // n << step;
void big_int_right_shift(const big_int *n, int step, big_int *dst); // n << step;
void big_int_multiply(const big_int *n1, const big_int *n2, big_int *result); // n1 * n2
void big_int_divide(const big_int *n1, const big_int *n2, big_int *quotient, big_int *remainder); // n1 = quotient * n2 + remainder
void big_int_mod_pow(const big_int *x, const big_int *y, const big_int *n, big_int *result); // x^y (mod n)

#endif // BIG_INT_H_INCLUDED
