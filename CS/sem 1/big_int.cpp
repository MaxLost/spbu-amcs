typedef struct big_int {
    unsigned int length;
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

void big_int_print_bin(const big_int *n) {
    int k = 0;
    for (int i = 0; i < 8; i++) {
        if ((n->number[n->length-1] & (1 << 7 - i)) ? 1 : 0){
            break;
        }
        k++;
    }
    for (int j = 0+k; j < 8; j++){
        printf("%d", (n->number[n->length-1] & (1 << 7 - j)) ? 1 : 0);
    }
    for (int i = n->length - 2; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            printf("%d", n->number[i] & (1 << 7 - j) ? 1 : 0);
        }
    }
}

big_int *big_int_get_bin(const char *bin_number){
    unsigned int num_len = len(bin_number, false);
    unsigned int len = (num_len / 8) + ((num_len & 7) > 0);
    big_int *result = (big_int*) malloc(sizeof(big_int));
    result->length = len;
    result->number = (unsigned char*) malloc(len);
    bin_number = bin_number + num_len + 7;
    for (int i = 0; i < len; i++){
        bin_number -= 8;
        char buffer = 0;
        for (int j = 0; j < 8; j++){
            if ((len == 1) && (j > num_len))
                break;
            if (*(bin_number - j) == '1')
                buffer |= (1 << j);
        }
        result->number[i] = buffer;
    }
    return result;
}

void big_int_assign(big_int *a, const big_int *b){
    if (a->number != NULL)
        a->number = (unsigned char*) realloc(a->number, b->length);
    else
        a->number = (unsigned char*) malloc(b->length);
    a->length = b->length;
    memcpy((void*) a->number, (void*) b->number, b->length);
    return;
}

void big_int_free(big_int *n){
    free(n->number);
    free(n);
    return;
}

int main(){

    return 0;
}
