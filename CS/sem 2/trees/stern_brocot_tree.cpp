#include <stdlib.h>
#include <iostream>
#include <cmath>

struct node {
    int num;
    int denum;
    node *left;
    node *right;
};

typedef struct node node;

node *create_node(int num, int denum, node *left = NULL, node *right = NULL){
    node *ptr = (node *) malloc(sizeof(node));
    ptr->num = num;
    ptr->denum = denum;
    ptr->left = left;
    ptr->right = right;
    return ptr;
}

void expand_tree(node *root, node *prev_left, node* prev_right, int n){
    if (n >= 0){
        root->left = create_node(root->num + prev_left->num, root->denum + prev_left->denum);
        root->right = create_node(prev_right->num + root->num, prev_right->denum + root->denum);
        expand_tree(root->left, prev_left, root, n - 1);
        expand_tree(root->right, root, prev_right, n - 1);
    }
    return;
}

node *construct_tree(int n){
    node *root = create_node(1, 1);
    node *prev_left = create_node(0, 1);
    node *prev_right = create_node(1, 0);
    expand_tree(root, prev_left, prev_right, n - 1);
    free(prev_left);
    free(prev_right);
    return root;
}

char *approximate_value(node *root, int n, double value){
    char *out = (char *) malloc(n + 1);
    out[n] = '\0';
    for (int i = 0; i < n; i++){
        double buffer = (double) root->num / root->denum;
        if (value < buffer){
            out[i] = 'L';
            root = root->left;
        }
        else {
            out[i] = 'R';
            root = root->right;
        }
    }
    return out;
}

int main(){
    // Golden ratio: 1.6180339887498948482
    // Pi:           3.14
    // e:            2.72
    //*
    int n = 24;
    node *tree = construct_tree(n);
    double golder_ratio = 1.6180339887498948482;
    char *result = approximate_value(tree, n, golder_ratio);
    printf("Golden ratio: %s\n", result);
    result = approximate_value(tree, n, M_PI);
    printf("Pi: %s\n", result);
    result = approximate_value(tree, n, M_E);
    printf("e: %s\n", result);
    //*/
    /*
    printf("Enter amount of digits in approximation: ");
    scanf("%d", &n);
    if (n <= 24){
        node *tree = construct_tree(n + 1);
        double value;
        printf("Enter value to approximate: ");
        scanf("%lf", &value);
        if (value <= 0){
            return 1;
        }
        char *approximation = approximate_value(tree, n, value);
        printf("%s", approximation);
    }
    else {
        return 1;
    }
    */
    return 0;
}
