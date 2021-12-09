#include <iostream>
#include <string.h>
#include "big_int.h"

unsigned int len(const char *num, bool rev){
    unsigned int k = 0;
    if (rev){
        for (int i = strlen(num); i >= 0; i--)
            if (num[i] >= '0' && num[i] <= '9')
                k++;
    }
    else {
        for (int i = 0; i < strlen(num); i++)
            if (num[i] >= '0' && num[i] <= '9')
                k++;
    }
    return k;
}


void big_int_print_dec(const big_int *n);
big_int *big_int_get_dec(const char *dec_number);

void big_int_left_shift(const big_int *n, int step, big_int *dst); // n << step;
void big_int_multiply(const big_int *n1, const big_int *n2, big_int *result); // n1 * n2
void big_int_divide(const big_int *n1, const big_int *n2, big_int *quotient, big_int *remainder); // n1 = quotient * n2 + remainder
void big_int_mod_pow(const big_int *x, const big_int *y, const big_int *n, big_int *result); // x^y (mod n)


void big_int_print_bin(const big_int *n) {
    int k = 0;
    if (n->sign == 0)
        printf("%c", '-');
    if (n->length == 1 && (n->number[0] == 0)){
        printf("%c", '0');
        return;
    }
    unsigned char mask = 0x80;
    for (; mask; mask >>= 1) {
        if ((n->number[n->length-1] & mask) ? 1 : 0){
            break;
        }
        k++;
    }
    mask = 0x80 >> k;
    for (; mask; mask >>= 1){
        printf("%d", (n->number[n->length-1] & mask) ? 1 : 0);
    }
    for (int i = n->length - 2; i >= 0; i--) {
        mask = 0x80;
        for (; mask; mask >>= 1){
            printf("%d", (n->number[i] & mask) ? 1 : 0);
        }
    }
}

// Working with limit on length of big_int
void big_int_print_dec(const big_int *n){
    int k = 0;
    for (int i = 0; i < 8; i++) {
        if (n->number[n->length-1] & (1 << 7 - i)){
            break;
        }
        k++;
    }
    unsigned long long result = 0;
    for (int i = 0; i < n->length-1; i++){
        for (int j = 0; j < 8; j++){
            if (n->number[i] & (1 << j))
                result += (1 << j+(8*i));
        }
    }
    for (int j = 0; j < 8-k; j++){
        if (n->number[n->length-1] & (1 << j))
            result += (1 << j+((n->length-1)*8));
    }
    printf("%lld", result);
    return;
}

big_int *big_int_get_bin(const char *bin_number){
    unsigned int num_len = len(bin_number, false);
    unsigned int len = (num_len >> 3) + ((num_len & 7) > 0);
    big_int *result = (big_int*) malloc(sizeof(big_int));
    result->length = len;
    result->number = (unsigned char*) malloc(len);
    int d = 0;
    if (bin_number[0] == '-'){
        result->sign = 0;
        ++bin_number;
    }
    else
        result->sign = 1;
    for (int i = 0; i < len; i++){
        unsigned char buffer = 0;
        for (int j = 0; j < 8; j++){
            if ((i == len - 1) && (j >= (num_len - (i * 8))))
                break;
            if (bin_number[num_len - 1 - ((i * 8) + j)] == '1')
                buffer |= (1 << j);
        }
        result->number[i] = buffer;
    }
    return result;
}

big_int *big_int_get_dec(const char *dec_number){
    unsigned int num_len = len(dec_number, false);
    char *num = (char*) malloc(num_len*sizeof(char));
    for (int i = 0; i < num_len; i++)
        *(num+i) = *(dec_number+i);

    char *buffer = (char*) malloc(num_len*4);
    int k = 0, buff_len = 0;
    while (k < num_len){
        int t = 0;
        (num[num_len-1] & 1) ? buffer[buff_len] = '1' : buffer[buff_len] = '0';
        buff_len++;
        //* Additional output of "num" values
        printf("\n");
        for (int i = 0; i < num_len; i++){
            printf("%c", num[i]);
        }
        //*/
        for (int i = k; i < num_len; i++){
            char current_digit = num[i] - '0';
            int next_t = 0;
            if (current_digit & 1)
                next_t = 5;
            current_digit >>= 1;
            current_digit += t;
            t = 0;
            if (current_digit > 9){
                num[i-1] += (current_digit / 10);
                current_digit %= 10;
            }
            t = next_t;
            num[i] = current_digit + '0';
        }
        if (num[k] == '0')
            k++;
    }
    for (int i = 0; i < buff_len / 2; i++){
        int temp = *(buffer+i);
        *(buffer+i) = *(buffer+buff_len-1-i);
        *(buffer+buff_len-1-i) = temp;
    }
    buffer = (char*) realloc(buffer, buff_len);
    big_int *result = big_int_get_bin(buffer);
    free(buffer);
    free(num);
    return result;
}

void big_int_clear(big_int *n){
    n->number = (unsigned char*) realloc(n->number, 1);
    n->length = 1;
    n->sign = 1;
    n->number[0] = 0;
}

void big_int_assign(big_int *dest, const big_int *src){
    big_int_clear(dest);
    dest->number = (unsigned char*) realloc(dest->number, src->length);
    dest->length = src->length;
    dest->sign = src->sign;
    memcpy((void*) dest->number, (void*) src->number, src->length);
    return;
}

void big_int_free(big_int *n){
    free(n->number);
    free(n);
    return;
}

void big_int_resize(big_int *n){
    unsigned int new_len = n->length;
    for (int i = n->length-1; i >= 0; i--)
        if (n->number[i] & 0xFF)
            break;
        else
            --new_len;
    if (new_len > 0){
        n->length = new_len;
    }
    else
        n->length = 1;
    n->number = (unsigned char *) realloc(n->number, n->length);
}

void big_int_transfer_operation(const big_int* n1, const big_int* n2, void (*operation)(const big_int *n1, const big_int *n2, big_int *result), big_int *result){
    big_int_clear(result);
    if (n1->sign > n2->sign) {
        big_int_assign(result, n2);
        result->sign = !result->sign;
        big_int *t = big_int_get_bin("0");
        operation(n1, result, t);
        big_int_assign(result, t);
        big_int_free(t);
    }
    else {
        big_int_assign(result, n1);
        result->sign = !result->sign;
        big_int *t = big_int_get_bin("0");
        operation(n2, result, t);
        big_int_assign(result, t);
        big_int_free(t);
    }
    return;
}

int big_int_compare(const big_int *n1, const big_int *n2, bool abs){ // n1 > n2 -> 1 / n1 == n2 -> 0 / n1 < n2 -> -1
    if (abs){
        if (n1->length > n2->length)
                return 1;
        else if (n2->length > n1->length)
                return -1;
        unsigned int len = n1->length;
        for (int i = len - 1; i >= 0; i--){
            unsigned char mask = 0x80;
            for (; mask; mask >>= 1)
                if ((n1->number[i] & mask) ^ (n2->number[i] & mask))
                    if (n1->number[i] & mask)
                        return 1;
                    else
                        return -1;
        }
        return 0;
    }
    else {
        if (n1->sign != n2->sign){
            if (n1->sign > n2->sign)
                return 1;
            else
                return -1;
        }
        else {
            if (n1->length > n2->length)
                return 1;
            else if (n2->length > n1->length)
                return -1;
            else {
                unsigned int len = n1->length;
                for (int i = len - 1; i >= 0; i--){
                    unsigned char mask = 0x80;
                    for (; mask; mask >>= 1)
                        if ((n1->number[i] & mask) ^ (n2->number[i] & mask))
                            if (n1->number[i] & mask)
                                return 1;
                            else
                                return -1;
                }
            }
        }
        return 0;
    }
}

void big_int_add(const big_int *n1, const big_int *n2, big_int *result){ // n1 + n2
    unsigned int len = (n1->length < n2->length) ? n2->length : n1->length;
    big_int_clear(result);
    result->number = (unsigned char*) realloc(result->number, len);
    result->length = len;
    if (n1->sign != n2->sign){
        big_int_clear(result);
        big_int_transfer_operation(n1, n2, &big_int_sub, result);
        return;
    }
    unsigned char transfer = 0;
    for (int i = 0; i < len; i++){
        unsigned char buffer = 0;
        for (int j = 0; j < 8; j++){
            unsigned char a = 0, b = 0;
            if (n1->length > i)
                a = ((n1->number[i] & (1 << j)) ? 1 : 0);
            if (n2->length > i)
                b = ((n2->number[i] & (1 << j)) ? 1 : 0);
            unsigned char current_digit = (a ^ b);
            if (transfer)
                current_digit ^= transfer;
            transfer = ((a & b) || (transfer && (a != b)));
            buffer |= (current_digit << j);
        }
        result->number[i] = buffer;
    }
    if (transfer){
        result->length++;
        result->number = (unsigned char*) realloc(result->number, len + 1);
        result->number[len] = 1;
    }
    if (n1->sign)
        result->sign = 1;
    else
        result->sign = 0;
    return;
}

void big_int_add_direct(big_int *n1, const big_int *n2){ // n1 = n1 + n2
    unsigned int len = (n1->length < n2->length) ? n2->length : n1->length;
    big_int *result = (big_int*) malloc(sizeof(big_int));
    result->number = (unsigned char*) malloc(len);
    result->length = len;
    if (n1->sign != n2->sign){
        big_int_clear(result);
        big_int_transfer_operation(n1, n2, &big_int_sub, result);
        return;
    }
    unsigned char transfer = 0;
    for (int i = 0; i < len; i++){
        unsigned char buffer = 0;
        for (int j = 0; j < 8; j++){
            unsigned char a = 0, b = 0;
            if (n1->length > i)
                a = ((n1->number[i] & (1 << j)) ? 1 : 0);
            if (n2->length > i)
                b = ((n2->number[i] & (1 << j)) ? 1 : 0);
            unsigned char current_digit = (a ^ b);
            if (transfer)
                current_digit ^= transfer;
            transfer = ((a & b) || (transfer && (a != b)));
            buffer |= (current_digit << j);
        }
        result->number[i] = buffer;
    }
    if (transfer){
        result->length++;
        result->number = (unsigned char*) realloc(result->number, len + 1);
        result->number[len] = 1;
    }
    if (n1->sign)
        result->sign = 1;
    else
        result->sign = 0;

    big_int_assign(n1, result);
    big_int_free(result);
    return;
}

void big_int_sub(const big_int *n1, const big_int *n2, big_int *result){ // n1 - n2
    unsigned int len = (n1->length < n2->length) ? n2->length : n1->length;
    big_int_clear(result);
    result->number = (unsigned char*) realloc(result->number, len);
    result->length = len;
    if (n1->sign != n2->sign){
        big_int_clear(result);
        big_int_transfer_operation(n1, n2, &big_int_add, result);
        return;
    }
    unsigned char transfer = 0;
    for (int i = 0; i < len; i++){
        unsigned char buffer = 0;
        for (int j = 0; j < 8; j++){
            unsigned char a = 0, b = 0;
            if (n1->length > i)
                a = ((n1->number[i] & (1 << j)) ? 1 : 0);
            if (n2->length > i)
                b = ((n2->number[i] & (1 << j)) ? 1 : 0);
            unsigned char current_digit = (a ^ b);
            current_digit ^= transfer;
            transfer = ((a == 0) && b) || (transfer && !(a ^ b));
            buffer |= (current_digit << j);
        }
        result->number[i] = buffer;
    }
    if (big_int_compare(n1, n2, false) < 0){
        result->sign = 1;
        for (int i = 0; i < len; i++){
            unsigned char buffer = 0;
            for (int j = 0; j < 8; j++)
                buffer |= (result->number[i] & (1 << j)) ? 0 : 1;
            result->number[i] = buffer;
        }
        big_int *one = big_int_get_bin("1");
        big_int *t = big_int_get_bin("0");
        big_int_add(result, one, t);
        big_int_assign(result, t);
        big_int_free(t);
        big_int_free(one);
        result->sign = 0;
    }
    else {
        result->sign = 1;
    }
    big_int_resize(result);
    return;
}

void big_int_left_shift(const big_int *n, int step, big_int *dst){ // n << step
    big_int_clear(dst);
    big_int_assign(dst, n);
    for (int k = 0; k < step; k++){
        unsigned int transfer = 0;
        for (int i = 0; i < dst->length; i++){
            unsigned char buffer = 0;
            for (int j = 0; j < 8; j++){
                buffer |= transfer << j;
                transfer = (dst->number[i] & (1 << j)) ? 1 : 0;
            }
            dst->number[i] = buffer;
        }
        if (transfer){
            dst->number = (unsigned char*) realloc(dst->number, dst->length + 1);
            dst->number[dst->length] = 1;
            dst->length++;
        }
    }

    return;
}

void big_int_right_shift(const big_int *n, int step, big_int *dst){ // n >> step
    big_int_clear(dst);
    big_int_assign(dst, n);
    for (int k = 0; k < step; k++){
        unsigned int transfer = 0;
        for (int i = dst->length - 1; i >= 0; i--){
            unsigned char buffer = 0;
            for (int j = 7; j >= 0; j--){
                buffer |= transfer << j;
                transfer = (dst->number[i] & (1 << j)) ? 1 : 0;
            }
            dst->number[i] = buffer;
        }
        big_int_resize(dst);
    }

    return;
}

void big_int_multiply(const big_int *n1, const big_int *n2, big_int *result){ // n1 * n2
    big_int_clear(result);
    big_int *temp = big_int_get_bin("0");
    big_int_assign(temp, n1);
    temp->sign = 1;
    for (int i = 0; i < n2->length; i++){
        //printf("\n");
        //big_int_print_bin(result);
        for (int j = 0; j < 8; j++){
            if (n2->number[i] & (1 << j)){
                big_int_add_direct(result, temp);
            }
            big_int *t = big_int_get_bin("0");
            big_int_left_shift(temp, 1, t);
            big_int_assign(temp, t);
            big_int_free(t);
        }
    }

    if (n1->sign == n2->sign)
        result->sign = 1;
    else
        result->sign = 0;
    big_int_free(temp);
    return;
}

void big_int_divide(const big_int *n1, const big_int *n2, big_int *quotient, big_int *remainder){ // n1 / n2 = quotient * n2 + remainder
    big_int_clear(quotient);
    big_int_clear(remainder);
    big_int_assign(remainder, n1);
    remainder->sign = 1;
    big_int *temp = big_int_get_bin("0");
    big_int_assign(temp, n2);
    temp->sign = 1;
    if (big_int_compare(remainder, temp, true) < 0){
        remainder->sign = n1->sign;
        big_int_assign(quotient, big_int_get_bin("0"));
        big_int_free(temp);
        return;
    }
    else if (big_int_compare(remainder, temp, true) == 0){
        big_int_assign(remainder, big_int_get_bin("0"));
        big_int_assign(quotient, big_int_get_bin("1"));
        big_int_free(temp);
        return;
    }
    int c = 0, d = 0;
    for (int j = 7; j >= 0; j--){
        if ((temp->number[temp->length - 1] & (1 << j)) && !c)
            c = j;
        if ((remainder->number[remainder->length - 1] & (1 << j)) && !d)
            d = j;
    }
    big_int *t = big_int_get_bin("0");
    big_int_left_shift(temp, ((remainder->length) - (temp->length))*8, t);
    big_int_assign(temp, t);
    if (c > d){
        big_int_right_shift(temp, (c-d), t);
        big_int_assign(temp, t);
    }
    else if (d > c){
        big_int_left_shift(temp, (d-c), t);
        big_int_assign(temp, t);
    }

    big_int *zero = big_int_get_bin("0");
    while ((big_int_compare(temp, n2, true) >= 0) && (big_int_compare(remainder, zero, true) >= 0)){
        big_int_clear(t);
        big_int_left_shift(quotient, 1, t);
        big_int_assign(quotient, t);
        if (big_int_compare(remainder, temp, true) >= 0){
            big_int_clear(t);
            big_int_sub(remainder, temp, t);
            big_int_assign(remainder, t);
            quotient->number[0] |= 1;
            big_int_resize(remainder);
        }
        big_int_clear(t);
        big_int_right_shift(temp, 1, t);
        big_int_assign(temp, t);
    }
    if (n1->sign != n2->sign){
        quotient->sign = 0;
        remainder->sign = 0;
        big_int_clear(t);
        big_int_add(remainder, n2, t);
        big_int_assign(remainder, t);
    }
    big_int_free(temp);
    big_int_free(t);
    big_int_free(zero);
    big_int_resize(quotient);
    big_int_resize(remainder);
    return;
}

void big_int_mod_pow(const big_int *x, const big_int *y, const big_int *n, big_int *result){
    //big_int_clear(result);
    big_int_free(result);
    result = big_int_get_bin("1");
    //big_int *temp = big_int_get_bin("1");
    //big_int_assign(result, temp);
    //big_int_free(temp);
    big_int *q = big_int_get_bin("0");
    big_int *deg = big_int_get_bin("0");

    big_int_divide(x, n, q, deg);
    for (int i = 0; i < y->length; i++){
        for (int j = 0; j < 8; j++){
            big_int *t = big_int_get_bin("0");
            if (y->number[i] & (1 << j)){
                //big_int_clear(t);
                big_int_multiply(result, deg, t);
                big_int_assign(result, t);
                //big_int_clear(t);
                big_int_divide(result, n, q, t);
                big_int_assign(result, t);
                //big_int_resize(result);
            }
            printf("\n%d) ", i*8+j);
            //big_int_clear(t);
            big_int_multiply(deg, deg, t);
            printf("\nend\n");
            big_int_assign(deg, t);
            //big_int_clear(t);
            big_int_divide(deg, n, q, t);
            big_int_assign(deg, t);
            //big_int_resize(deg);
            big_int_free(t);
        }
    }
    big_int *t = big_int_get_bin("0");
    big_int_divide(result, n, q, t);
    big_int_assign(result, t);
    big_int_free(deg);
    big_int_free(t);
    big_int_free(q);
    return;
}

int main(){
    //for (unsigned long long i; i < 10e16; i++){
    const char *bin_str1 = "111"; // 11010101'01010101
    //const char *bin_str2 = "11100011000110011"; // 10101010'10101011
    const char *bin_str3 = "1001010111"; //349
    const char *bin_str4 = "1111010110110011011101100100010011010110000101010";
    const char *bin_str5 = "-10110111";
    big_int *q = big_int_get_bin("0");

    big_int *x = big_int_get_bin(bin_str3);
    big_int *y = big_int_get_bin(bin_str4);
    big_int *n = big_int_get_bin("1111010110110011011101100100010011010110000101011");
    big_int *t = big_int_get_bin("0");
    big_int *a = big_int_get_bin("0");
    //big_int_multiply(y, n, t);
    big_int_mod_pow(x, y, n, t);
    //big_int_divide(t, y, x, a);
    big_int_print_bin(t); // answer = 16

    big_int_free(x);
    big_int_free(y);
    big_int_free(n);
    big_int_free(t);

    // division quotient = 1100, remainder = 10
    /*
    big_int* a = big_int_get_bin(bin_str3);
    big_int* b = big_int_get_bin(bin_str4);
    printf("\n");
    big_int_print_bin(a);
    printf("\n");
    big_int_print_bin(b);
    printf("\n");
    big_int *t = big_int_get_bin("0");
    big_int_multiply(a, b, t);
    big_int_print_bin(t);
    /*big_int* t = big_int_get_bin("0");
    printf("\n");
    big_int_print_bin(big_int_sub(a, b));
    printf("\n");
    big_int_print_bin(big_int_add(b, t));
    big_int_free(t);
    big_int_free(a);
    big_int_free(b);
    */
    /*
    big_int *mlt =  big_int_get_bin(bin_str5);
    big_int *tst = big_int_get_bin(bin_str1);
    printf("\n");
    big_int_print_bin(tst);
    printf("\n");
    big_int_print_bin(mlt);
    printf("\n");
    big_int *q = big_int_get_bin("0");
    big_int *r = big_int_get_bin("0");
    big_int_divide(mlt, tst, q, r);
    printf("\n");
    big_int_print_bin(q);
    printf(" ");
    big_int_print_bin(r);
    //*/
    //}
    return 0;
}
