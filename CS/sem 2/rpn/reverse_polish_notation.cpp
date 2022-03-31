#include <iostream>
#include <cstring>
#include "str_input.h"

#define EZERODIV 101
#define EUNEXPSYMB 201
#define EBRACKET 202
#define EOPCOUNT 203
#define EUNDEF 999

struct node {
    union {
        int number;
        char operation;
    };
    char type;          // 0 - operation, 1 - number
    node* next;
};

typedef struct node node;

void error_handler(int error_type, void* data = NULL, void* error_position = NULL) {
    switch (error_type) {                                // 1** - calculation errors; 2** - errors in rpn
    case EZERODIV: {
        printf("\nERROR: Zero division error\n");
        data = NULL;
    } break;
    case EUNEXPSYMB: {
        printf("\nERROR: Unexpected symbol\n");
        if (data) {
            printf("%s\n", (char*)data);
        }
        if (data && error_position) {
            char* ptr = (char*)data;
            while (*ptr != '\0') {
                if (ptr == (char*)error_position) {
                    printf("^");
                }
                else {
                    printf("~");
                }
                ++ptr;
            }
            printf("\n");
        }
    } break;
    case EBRACKET: {
        printf("\nERROR: Incorrect brackets/comma use\n");
        if (data) {
            printf("%s\n", (char*)data);
        }
        if (data && error_position) {
            char* ptr = (char*)data;
            while (*ptr != '\0') {
                if (ptr == (char*)error_position) {
                    printf("^");
                }
                else {
                    printf("~");
                }
                ++ptr;
            }
            printf("\n");
        }
    } break;
    case EOPCOUNT: {
        printf("\nERROR: Too few arguments for operation\n");
        data = NULL;
    } break;
    case EUNDEF:
    default:
        printf("\nERROR: Undefined error\n");
        break;
    }
}

node* node_create(char operation, char type, node* next = NULL) {
    node* t = (node*)malloc(sizeof(node));
    t->type = type;
    if (!type)
        t->operation = operation;
    else
        return NULL;
    t->next = next;
    return t;
}

node* node_create(int number, char type, node* next = NULL) {
    node* t = (node*)malloc(sizeof(node));
    t->type = type;
    if (type)
        t->number = number;
    else
        return NULL;
    t->next = next;
    return t;
}

int gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;
    while (a % b != 0) {
        if (b > a) {
            std::swap(a, b);
        }
        a = a % b;
    }
    return b;
}

int lcm(int a, int b) {
    int k = gcd(a, b);
    return (a * b) / k;
}

int calculate_rpn(node** rpn) {
    node* iter = *rpn;
    node *l = NULL, *r = NULL, *prev = NULL;
    int buffer = 0;
    while (iter){
        if (!iter->type) {
            if (r && l) {
                switch (iter->operation) {
                    case '+': buffer = l->number + r->number; break;
                    case '*': buffer = l->number * r->number; break;
                    case 'g': buffer = gcd(l->number, r->number); break;
                    case 'l': buffer = lcm(l->number, r->number); break;
                    case '/':
                        if (r->number == 0) {
                            error_handler(EZERODIV);
                            return 1;
                        }
                        buffer = l->number / r->number; break;
                    default: error_handler(EUNDEF);
                };
                if (iter->next) {
                    l->next = iter->next;
                    l->number = buffer;
                    free(iter); free(r);
                    r = l;
                    iter = l->next;
                    l = prev;
                }
                else {
                    l->next = NULL;
                    l->number = buffer;
                    free(iter); free(r);
                    r = l;
                    iter = l->next;
                    l = prev;
                }
            }
            else {
                error_handler(EOPCOUNT);
                return 1;
            }
        }
        else {
            prev = l;
            l = r;
            r = iter;
            iter = iter->next;
        }
        /*node* k = *rpn;
        printf("\n");
        while (k){
            if (k->type){
                printf("%d, ", k->number);
            }
            else {
                printf("%c, ", k->operation);
            }
            k = k->next;
        }*/
    }
    return 0;
}

void queue_push(node** tail, node* element) {
    (*tail)->next = element;
    element->next = NULL;
    *tail = element;
    return;
}

void op_stack_insert(node** op_stack, node** out, const char op) {
    if ((*op_stack)->operation == '\0') {
        (*op_stack)->operation = op;
        (*op_stack)->type = 0;
    }
    else {
        node* tail = *op_stack;
        if (op == '+') {
            while (tail->operation == '*' || tail->operation == '/') {
                if (tail->next) {
                    *op_stack = (*op_stack)->next;
                    queue_push(out, tail);
                    tail = *op_stack;
                }
                else {
                    node* t = node_create(tail->operation, 0, *out);
                    queue_push(out, t);
                    tail->operation = '\0';
                }
            }
            if (tail->operation != '*' && tail->operation != '/') {
                if (tail->operation == '\0') {
                    tail->operation = op;
                }
                else {
                    node* t = node_create(op, 0, tail);
                    (*op_stack) = t;
                }
            }
        }
        else if (op == '*' || op == '/' || op == '(' || op == 'g' || op == 'l') {
            node* t = node_create(op, 0, tail);
            (*op_stack) = t;
        }
    }
    return;
}

void insert_num(node** tail, const int num) {
    if ((*tail)->number == 0) {
        (*tail)->type = 1;
        (*tail)->number = num;
    }
    else {
        node* t = node_create(num, 1);
        queue_push(tail, t);
    }
    return;
}

node* get_rpn(const char* str) {
    node* head = node_create(0, 1, NULL);
    node* out = head;
    node* op_stack = node_create('\0', 0, NULL);

    char* s = (char*)str;
    int buffer = 0, sign = 1;
    while (*s != '\0') {
        if (*s >= '0' && *s <= '9') {
            buffer = strtol(s, &s, 10);
            buffer *= sign;
            sign = 1;
            insert_num(&out, buffer);
            buffer = 0;
        }
        else {
            switch (*s) {
            case '-':
                if (s != str && *(s - 1) != '(') {
                    op_stack_insert(&op_stack, &out, '+');
                }
                sign *= -1;
                break;
            case '+':
            case '*':
            case '/':
            case '(': op_stack_insert(&op_stack, &out, *s); break;
            case 'g': {
                if (*(s + 1) == 'c' && *(s + 2) == 'd' && *(s + 3) == '(') {
                    op_stack_insert(&op_stack, &out, 'g');
                    s += 3;
                }
                else {
                    if (*(s + 1) != 'c') {
                        error_handler(201, (void*)str, (void*)(s + 1));
                    }
                    else if (*(s + 2) != 'd') {
                        error_handler(201, (void*)str, (void*)(s + 2));
                    }
                    else if (*(s + 3) != '(') {
                        error_handler(201, (void*)str, (void*)(s + 3));
                    }
                    return NULL;
                }
            } break;
            case 'l': {
                if (*(s + 1) == 'c' && *(s + 2) == 'm' && *(s + 3) == '(') {
                    op_stack_insert(&op_stack, &out, 'l');
                    s += 3;
                }
                else {
                    if (*(s + 1) != 'c') {
                        error_handler(201, (void*)str, (void*)(s + 1));
                    }
                    else if (*(s + 2) != 'm') {
                        error_handler(201, (void*)str, (void*)(s + 2));
                    }
                    else if (*(s + 3) != '(') {
                        error_handler(201, (void*)str, (void*)(s + 3));
                    }
                    return NULL;
                }
            } break;
            case ',': {
                node* t = op_stack;
                while (t->operation != 'g' && t->operation != 'l') {
                    if (t->next) {
                        op_stack = op_stack->next;
                        queue_push(&out, t);
                        t = op_stack;
                    }
                    else {
                        node* tmp = node_create(t->operation, 0);
                        queue_push(&out, tmp);
                        t->operation = '\0';
                    }
                    if (!t || t->operation == '\0') {
                        error_handler(202, (void*)str, (void*)s);
                        return NULL;
                    }
                }
            } break;
            case ')': {
                node* t = op_stack;
                while (t->operation != '(' && t->operation != 'g' && t->operation != 'l') {
                    if (t->next) {
                        op_stack = op_stack->next;
                        queue_push(&out, t);
                        t = op_stack;
                    }
                    else {
                        node* tmp = node_create(t->operation, 0);
                        queue_push(&out, tmp);
                        t->operation = '\0';
                    }
                    if (!t || t->operation == '\0') {
                        error_handler(202, (void*)str, (void*)s);
                        return NULL;
                    }
                }
                if (t->operation == '(') {
                    if (op_stack) {
                        op_stack = op_stack->next;
                        free(t);
                    }
                    else {
                        op_stack->operation = '\0';
                    }
                }
                else {
                    t = node_create(op_stack->operation, 0);
                    queue_push(&out, t);
                    if (op_stack->next) {
                        t = op_stack;
                        op_stack = op_stack->next;
                        free(t);
                    }
                    else {
                        op_stack->operation = '\0';
                    }
                }
            } break;
            default:
                error_handler(201, (void*)str, (void*)s);
                return NULL;
            }
            ++s;
        }
    }
    while (op_stack && op_stack->operation != '\0') {
        node* t = op_stack;
        if (t->operation == '(') {
            error_handler(202, (void*)str);
            return NULL;
        }
        op_stack = op_stack->next;
        queue_push(&out, t);
    }
    return head;
}

int main() {
    char* input = get_string_input();
    node* t = get_rpn(input);
    node* k = t;
    while (k) {
        if (k->type == 1) {
            printf("%d, ", k->number);
        }
        else if (k->type == 0) {
            printf("%c, ", k->operation);
        }
        k = k->next;
    }
    int status = 1;
    if (t)
        status = calculate_rpn(&t);
    if (!status && t)
        printf("\n%d", t->number);
}
