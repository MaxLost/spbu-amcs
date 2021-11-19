#include <iostream>
#include <string.h>

typedef struct big_int {
    unsigned int length;
    unsigned char sign; // 1 => '+', 0 => '-'
    unsigned char *number;
} big_int;

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


void big_int_print_bin(const big_int *n);
void big_int_print_dec(const big_int *n);
big_int *big_int_get_bin(const char *bin_number);
big_int *big_int_get_dec(const char *dec_number);
void big_int_clear(big_int *n);
void big_int_assign(big_int *a, const big_int *b);
void big_int_free(big_int *n);
void big_int_resize(big_int *n);
big_int* big_int_transfer_operation(const big_int* n1, const big_int* n2, big_int* (*operation)(const big_int *n1, const big_int *n2));
int big_int_compare(const big_int *n1, const big_int *n2);  // n1 >= n2 -> 1 / n1 < n2 -> 0
big_int* big_int_add(const big_int *n1, const big_int *n2); // n1 + n2
big_int* big_int_sub(const big_int *n1, const big_int *n2); // n1 - n2

void big_int_print_bin(const big_int *n) {
    int k = 0;
    if (n->sign == 0)
        printf("%c", '-');
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
    unsigned int len = (num_len / 8) + ((num_len & 7) > 0);
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

void big_int_assign(big_int *a, const big_int *b){
    big_int_clear(a);
    a->number = (unsigned char*) realloc(a->number, b->length);
    a->length = b->length;
    a->sign = b->sign;
    memcpy((void*) a->number, (void*) b->number, b->length);
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
    n->length = new_len;
    n->number = (unsigned char *) realloc(n->number, new_len);
}

big_int* big_int_transfer_operation(const big_int* n1, const big_int* n2, big_int* (*operation)(const big_int *n1, const big_int *n2)){
    big_int *t = big_int_get_bin("0");
    if (n1->sign > n2->sign) {
        big_int_assign(t, n2);
        t->sign = !t->sign;
        t = operation(n1, t);
    }
    else {
        big_int_assign(t, n1);
        t->sign = !t->sign;
        t = operation(n2, t);
    }
    return t;
}

int big_int_compare(const big_int *n1, const big_int *n2){ // n1 >= n2 -> 1 / n1 < n2 -> 0
    if (n1->sign != n2->sign){
        if (n1->sign > n2->sign)
            return 1;
        else
            return 0;
    }
    else {
        if (n1->length > n2->length)
            return 1;
        else if (n2->length > n1->length)
            return 0;
        else {
            unsigned int len = n1->length;
            for (int i = len - 1; i >= 0; i--){
                unsigned char mask = 0x80;
                for (; mask; mask >>= 1)
                    if ((n1->number[i] & mask) ^ (n2->number[i] & mask))
                        if (n1->number[i] & mask)
                            return 1;
                        else
                            return 0;
            }
        }
    }
    return 1;
}

big_int* big_int_add(const big_int *n1, const big_int *n2){ // n1 + n2
    unsigned int len = (n1->length < n2->length) ? n2->length : n1->length;
    big_int *result = (big_int*) malloc(sizeof(big_int));
    result->number = (unsigned char*) malloc(len);
    result->length = len;
    if (n1->sign != n2->sign){
        big_int_clear(result);
        result = big_int_transfer_operation(n1, n2, &big_int_sub);
        return result;
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
    return result;
}

big_int* big_int_sub(const big_int *n1, const big_int *n2){ // n1 - n2
    unsigned int len = (n1->length < n2->length) ? n2->length : n1->length;
    big_int *result = (big_int*) malloc(sizeof(big_int));
    result->number = (unsigned char*) malloc(len);
    result->length = len;
    if (n1->sign != n2->sign){
        big_int_clear(result);
        result = big_int_transfer_operation(n1, n2, &big_int_add);
        return result;
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
    if (big_int_compare(n1, n2) == 0){
        result->sign = 1;
        for (int i = 0; i < len; i++){
            unsigned char buffer = 0;
            for (int j = 0; j < 8; j++)
                buffer |= (result->number[i] & (1 << j)) ? 0 : 1;
            result->number[i] = buffer;
        }
        big_int *one = big_int_get_bin("1");
        result = big_int_add(result, one);
        big_int_free(one);
        result->sign = 0;
    }
    else {
        result->sign = 1;
    }
    big_int_resize(result);
    return result;
}

/*big_int* big_int_left_shift(const big_int *n, int step){
    big_int *result = big_int_get_bin("0");
    unsigned int new_len = n->length + (step / 8) + ((step & 7) > 0);
    result->length = new_len;
    result->number = (unsigned char*) realloc(result->number, new_len);
    int i = 0, j = 0;
    for (int i = 0; i <= new_len - n->length; i++){
        unsigned char buffer = 0;
        for (int j = 0; (j < 8) && (i * 8 + j < step); j++);
        result->number[i] = buffer;
    }

}*/

int main(){
    //const char *bin_str1 = "11100011000110001"; // 11010101'01010101
    //const char *bin_str2 = "11100011000110011"; // 10101010'10101011
    const char *bin_str3 = "101010101010101010101010";
    const char *bin_str4 = "101010101010111010101011";
    big_int *a = big_int_get_bin(bin_str3);
    big_int *b = big_int_get_bin(bin_str4);
    //printf("%d\n", a->sign);
    printf("\n");
    big_int_print_bin(a);
    printf("\n");
    big_int_print_bin(b);
    printf("\n");
    big_int *t = big_int_sub(a, b);
    //printf("\n%s\n", "11101011");
    big_int_print_bin(t);
    printf("\n");
    big_int_print_bin(big_int_add(b, t));
    big_int_free(a);
    big_int_free(b);
    //big_int *brrr = big_int_get_bin(bin_str3);
    //big_int_print_bin(brrr);

    return 0;
}
