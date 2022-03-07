#include <iostream>
#include <cstring>

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
