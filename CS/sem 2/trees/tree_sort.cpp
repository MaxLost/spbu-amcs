#include <iostream>

struct node {
    int number;
    node *left;
    node *right;
};

typedef struct node node;

node *create_node(int number, node *left = NULL, node *right = NULL){
    node *ptr = (node *) malloc(sizeof(node));
    ptr->number = number;
    ptr->left = left;
    ptr->right = right;
    return ptr;
}

void add_node(node *ptr, int n){
    while (true){
        if (n < ptr->number){
            if (ptr->left){
                ptr = ptr->left;
            }
            else {
                ptr->left = create_node(n);
                break;
            }
        }
        else {
            if (ptr->right){
                ptr = ptr->right;
            }
            else {
                ptr->right = create_node(n);
                break;
            }
        }
    }
    return;
}

void print_tree(node *root){
    if (root){
        print_tree(root->left);
        printf("%d ", root->number);
        print_tree(root->right);
    }
    return;
}

void get_sorted_list(node *root, int **sorted){
    if (root){
        get_sorted_list(root->left, sorted);
        **sorted = root->number;
        ++(*sorted);
        get_sorted_list(root->right, sorted);
    }
    return;
}

node *construct_tree(int n, int *num_list){
    node *root = create_node(num_list[0]);
    for (int i = 1; i < n; i++){
        add_node(root, num_list[i]);
    }
    return root;
}

int main(){
    int n;
    printf("Enter amount of elements: " );
    scanf("%d", &n);
    int *num_list = (int *) malloc(n*sizeof(int));
    printf("Enter element values: ");
    for (int i = 0; i < n; i++){
        scanf("%d", &num_list[i]);
    }
    node *tree = construct_tree(n, num_list);
    int *sorted = (int *) calloc(n, sizeof(int));
    int *t = sorted;
    get_sorted_list(tree, &sorted);
    sorted = t;
    for (int i = 0; i < n; i++){
        printf("%d ", sorted[i]);
    }
    return 0;
}
