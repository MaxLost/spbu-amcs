#include <iostream>
using namespace std;

long long fib_check(unsigned long n){
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long prev = 0, cur = 1, next;
    for (long i = 2; i <= n; i++){
        next = prev + cur;
        prev = cur;
        cur = next;
    }
    return cur;
}

void create_matrix(long (*matrix)[4]){
    *(matrix[0]) = 1;
    *(matrix[0]+1) = 1;
    *(matrix[0]+2) = 1;
    *(matrix[0]+3) = 0;
    return;
}

void copy_matrix(long (*from)[4], long (*to)[4]){
    *(to[0]) = *(from[0]);
    *(to[0]+1) = *(from[0]+1);
    *(to[0]+2) = *(from[0]+2);
    *(to[0]+3) = *(from[0]+3);
    return;
}

void multiply_matrixes(long (*a)[4], long (*b)[4], long (*out)[4], int m){
    *(out[0]) = (*(a[0]) * *(b[0]) + *(a[0] + 1) * *(b[0] + 2)) % m;
    *(out[0] + 1) = (*(a[0]) * *(b[0] + 1) + *(a[0] + 1) * *(b[0] + 3)) % m;
    *(out[0] + 2) = (*(a[0] + 2) * *(b[0]) + *(a[0] + 3) * *(b[0] + 2)) % m;
    *(out[0] + 3) = (*(a[0] + 2) * *(b[0] + 1) + *(a[0] + 3) * *(b[0] + 3)) % m;
    return;
}
/*
void get_matrix_pow(long (*matrix)[4], int power, int m){
    if (power < 1)
        return;
    long temp[4];
    copy_matrix(matrix, &temp);

    get_matrix_pow(&temp, power-1, m);

    multiply_matrixes(&temp, &temp, matrix, m);
    return;
}
*/
long fibonacci(unsigned long n, int m){
    if (n == 0) return 0;
    if (n == 1) return 1 % m;
    long ans[4], x[4];
    create_matrix(&ans);
    create_matrix(&x);
    while (n > 0){ // 10 = 8 + 2 = 2^3 + 2^1
        long temp[4];
        create_matrix(&temp);
        if (n & 1){
            multiply_matrixes(&ans, &x, &temp, m);
            copy_matrix(&temp, &ans);
        }
        create_matrix(&temp);
        multiply_matrixes(&x, &x, &temp, m);
        copy_matrix(&temp, &x);
        n >>= 1;
    }
    return ans[3];
}

int main(){
    unsigned long n;
    int m;
    cin >> n >> m;
    cout << fibonacci(n, m) << endl;
    long long x = fib_check(n);
    cout << x << ' ' << m << ' ' << x % m << endl;
    return 0;
}
