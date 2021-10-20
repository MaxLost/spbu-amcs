#include <iostream>
#include "extended_euclid.h"
int euclid(int a, int b){
    if (b > a){
        int t = a;
        a = b;
        b = t;
    }
    while (a && b){
        int t = b;
        b = a % b;
        a = t;
    }
    return (a+b);
}

int chinese_remainder1(const int *a, const int *m, int k){
    int M = 1;

    // Checking that solution is exist, if not exists -> returns -1
    for (int i = 0; i < k-1; i++){
        for (int j = i+1; j < k; j++){
            int r = euclid(*(m+i), *(m+j));
            if (r != 1)
                return -1;
        }
        M *= *(m+i);
    }

    M *= *(m+k-1);
    int ans = 0;
    for (int i = 0; i < k; i++){
        int Mi = M / *(m+i);
        int Mi_inv = mul_inverse(Mi, *(m+i));
        ans += *(a+i) * Mi * Mi_inv;
    }
    return ans % M;
}

int chinese_remainder2(const int *a, const int *m, int k){
    // Checking that solution is exist, if not exists -> returns -1
    for (int i = 0; i < k-1; i++){
        for (int j = i+1; j < k; j++){
            int r = euclid(*(m+i), *(m+j));
            if (r != 1)
                return -1;
        }
    }

    int prev_a = *a;
    int prev_m = *m;
    for (int i = 1; i < k; i++){
        int c, d;
        extended_euclid(prev_m, *(m+i), &c, &d);
        prev_a = *(a+i) * prev_m * c + prev_a * *(m+i) * d;
        prev_m = prev_m * *(m+i);
    }

    prev_a %= prev_m;
    if (prev_a < 0)
        prev_a += prev_m;
    return prev_a;
}

/*  TEST CASES
 *  1) Input:
 *  1 2 6
 *  2 3 7
 *  Output:
 *  41
 *
 *  2) Input:
 *  1 2 3 4 5
 *  2 3 5 7 11
 *  Output:
 *  1523
 */

int main(){
    int k;
    scanf("%d", &k);
    int a[k], m[k];
    printf("Enter a[i]: ");
    for (int i = 0; i < k; i++){
        scanf("%d", &a[i]);
    }
    printf("Enter m[i]: ");
    for (int i = 0; i < k; i++){
        scanf("%d", &m[i]);
    }
    int x = chinese_remainder1(&a[0], &m[0], k);
    printf("%d", x);
}
