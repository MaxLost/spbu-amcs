#ifndef BIG_INT_H_INCLUDED
#define BIG_INT_H_INCLUDED

/********************************************************************
 * Realization of data structure and functions for long arithmetics *
 * Created by Maksim Tisheninov, 2021                               *
 ********************************************************************/

typedef struct big_int {
    unsigned int length;
    unsigned char sign; // 1 = '+', 0 = '-'
    unsigned char *number;
} big_int;

void big_int_print_bin(const big_int *n);

/* Constructor for big_int data structure / Takes binary representation of number in string / Returns pointer on big_int item */
big_int *big_int_get_bin(const char *bin_number);

/* Function for reset a big_int item */
void big_int_clear(big_int *n);

/* Function for assigning value of the second big_int item to first / Two big_int items, first is destination, second is source */
void big_int_assign(big_int *dest, const big_int *src);

/* Function for correct freeing memory from big_int item / Takes big_int item that will be deleted */
void big_int_free(big_int *n);

/* Function for optimizing memory allocated for big_int item / Takes big_int item that will be optimized */
void big_int_resize(big_int *n);

/* Function for comparing 2 big_int values / Takes two big_int items that will be compared and argument that defines type of comparison: normal or absolute.
 * Normal type of comparison includes comparison by signs of numbers. Absolute type compares numbers only by absolute values.
 * Returns an integer value from -1 to 1. Result meaning: 1 -> first > second, 0 -> first == second, -1 -> first < second.
 */
int big_int_compare(const big_int *n1, const big_int *n2, bool abs);

/* Function for adding big_int values / Takes two big_int items that will be used as operands and big_int item in which result will be returned /
 * Returns result of adding second value to first.
 */
void big_int_add(const big_int *n1, const big_int *n2, big_int *result);

/* Function for adding big_int values / Takes two big_int items / Result of adding second value to first will be returned in the first argument */
void big_int_add_direct(big_int *n1, const big_int *n2);

/* Function for subtracting one big_int value from another / Takes two big_int items that will be used as operands and big_int item in which result will be returned /
 * Returns result of subtracting second value from first.
 */
void big_int_sub(const big_int *n1, const big_int *n2, big_int *result);

/* Function for adding N bits to the end of big_int value / Takes big_int item, integer number of bits that will be added and big_int item
 * in which result will be returned / Returns big_int value that was taken with N "0" bits at the end.
 */
void big_int_left_shift(const big_int *n, int step, big_int *result);

/* Function for deleting N bits from the end of big_int value / Takes big_int item, integer number of bits that will be added and big_int item
 * in which result will be returned / Returns big_int value that was taken without N bits at the end.
 */
void big_int_right_shift(const big_int *n, int step, big_int *result);

/* Function for multiplying two big_int values / Takes two big_int items that will be used as operands and big_int item in which result will be returned /
 * Returns result of multiplication of two taken big_int values
 */
void big_int_multiply(const big_int *n1, const big_int *n2, big_int *result);

/* Function for dividing one big_int value on another / Takes two big_int items that will be used as operands, big_int item in which quotient will be returned and
 * big_int item in which remainder will be returned / Returns quotient and remainder corresponds to this formula: n1 = quotient * n2 + remainder
 */
void big_int_divide(const big_int *n1, const big_int *n2, big_int *quotient, big_int *remainder);

/* Function for raising big_int value in degree of another big_int value modulo big_int value / Takes big_int item that will be raised in degree, big_int item that
 * will be used as degree value, big_int item that will be used as modulo value and big_int item in which result will be returned /
 * Returns result of raising x in degree y modulo n.
 */
void big_int_mod_pow(const big_int *x, const big_int *y, const big_int *n, big_int *result); // x^y (mod n)

#endif // BIG_INT_H_INCLUDED
