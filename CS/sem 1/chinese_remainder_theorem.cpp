#include <iostream>
#include "extended_euclid.h"
int euclid(int a, int b){
    if (b > a)
        swap(a, b);
    while (a && b){
        a = a % b;
        swap(a, b);
    }
    return (a+b);
}

int chinese_remainder1(const int *a, const int *m, int k){
    int M = 1;
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
        int m_inv = mul_inverse(Mi, *(m+i));
        ans += *(a+i) * Mi * m_inv;
    }
    return ans % M;
}

int chinese_remainder2(const int *a, const int *m, int k){
    for (int i = 0; i < k-1; i++){
        for (int j = i+1; j < k; j++){
            int r = euclid(*(m+i), *(m+j));
            if (r != 1)
                return -1;
        }
    }
    int prev_a = a[0];
    int prev_m = m[0];
    for (int i = 1; i < k; i++){
        int c, d;
        extended_euclid(prev_m, m[i], &c, &d);
        prev_a = a[i]*prev_m*c + prev_a*m[i]*d;
        prev_m = prev_m * m[i];
    }
    prev_a %= prev_m;
    if (prev_a < 0)
        prev_a += prev_m;
    return prev_a;
}
// 1 2 6; 2 3 7 => 41
int main(){
    int k;
    cin >> k;
    int a[k], m[k];
    printf("Enter a[i]: ");
    for (int i = 0; i < k; i++){
        cin >> a[i];
    }
    printf("Enter m[i]: ");
    for (int i = 0; i < k; i++){
        cin >> m[i];
    }
    int x = chinese_remainder1(&a[0], &m[0], k);
    printf("%d", x);
}
