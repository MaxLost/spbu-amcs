#include <iostream>
#include <cmath>
#include "legendre_euler.h"
#include "mod_pow.h"

int is_prime(long n){
    long root = sqrt(n);
    for (long i = 2; i <= root; i++){
        if (n % i == 0)
            return 0;
    }
    if ( (long) sqrt(n)*sqrt(n) == n)
        return 0;
    return 1;
}

int quadratic_congruence(long a, long p){
    long c = 2, k = 0, b = 1, _p = p - 1;
    while ((_p & 1) == 0){
        k++;
        _p >>= 1;
    }
    while (legendre_euclidian(c, p) == 1)
        c++;
    long r, h = _p;
    if (k >= 1)
        r = k - 1;
    else
        r = k;

    while (r >= 0){
        for (int i = 0; i <= r; i++){
            long expr = (mod_pow(a, pow(2, i) * h, p) * mod_pow(b, pow(2, i + 1), p)) % p;
            if (expr == 1){
                r = i;
                break;
            }
        }
        b = (b * mod_pow(c, h * (pow(2, k - r - 1)), p)) % p;
        r--;
    }

    long x = (mod_pow(a, (h+1) / 2, p) * b) % p;
    return x;
}

int main(){
    long a, p;
    printf("Enter A, P: ");
    scanf("%ld %ld", &a, &p);
    if (is_prime(p)){
        if (legendre_euclidian(a, p) == -1)
            printf("No solutions");
        else {
            int ans = quadratic_congruence(a, p);
            printf("%ld %ld", ans, -1*ans);
        }
    }
    else {
        printf("P is not prime");
    }
    return 0;
}
