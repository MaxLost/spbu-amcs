#include <iostream>
#include "extended_euclid.h"

int linear_diophantine(const int *a, int *x, int n, int b){
    int gcd = extended_euclid(*a, *(a+1), x, x+1);
    for (int k = 2; k < n; k++){
        int t = extended_euclid(gcd, *(a+k), &gcd, x+k);
        for (int i = 0; i < k; i++){
            *(x+i) *= gcd;
        }
        gcd = t;
    }
    if (b % gcd)
        return b % gcd;
    else {
        b = b / gcd;
        for (int i = 0; i < n; i++)
            *(x+i) *= b;
        return 0;
    }
}

/* Test cases
n = 3
6 15 10 1 => 0 6 -3 1

n = 4
64 48 46 5 1 => -6 6 2 1
33 33 33 33 5 => 5 (No solution)

n = 5
136 40 36 58 27 1 => 1456 -5096 182 -13 1
*/

int main(){
    int n;
    cin >> n;
    int b, a[n], x[n];
    for (int i = 0; i < n; i++)
        cin >> a[i];
    cin >> b;
    int status = linear_diophantine(&a[0], &x[0], n, b);
    if (status == 0){
        int ans = 0;
        for (int i = 0; i < n; i++){
            printf("%d ", x[i]);
            ans += a[i]*x[i];
        }
        if (ans == b)
            printf("\nCorrect");
        else
            printf("\nIncorrect");
    }
    else {
        printf("%d", status);
    }
}
