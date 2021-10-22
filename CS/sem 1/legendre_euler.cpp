#include <iostream>
#include <time.h>
#include <assert.h>
#include "mod_pow.h"
#define NUMBER_OF_ITERATIONS 10e6

int legendre_euler(long long a, long long p){
    long long x = mod_pow(a, ((p-1)/2), p);
    if (x != 0 && x != 1)
        return -1;
    else
        return x;
}

int legendre_euclidian(long long a, long long p){
    int ans = 1;
    if (a % p == 0)
        return 0;
    while (a != 0){
        int k = 0;
        while ((a & 1) == 0){
            k++;
            a >>= 1;
        }

        if (k & 1 && k != 0){
            if ((p & 7) == 3 || (p & 7) == 5)
                ans *= -1;
        }
        if ((a & 3) == 3 && (p & 3) == 3)
            ans *= -1;
        long long c = a;
        a = p % a;
        p = c;
    }
    return ans;
}

int main(){
    /*// TEST CORRECTNESS OF ALGORITHM
    long long a, p;
    printf("Enter a, p: ");
    scanf("%lld %lld", &a, &p);
    printf("Legendre-Euler: %d\n", legendre_euler(a, p));
    printf("Legendre quadratic reciprocity and euclidian division: %d\n", legendre_euclidian(a, p));
    */

    //  TIME TEST FOR TWO ALGORITHMS
    long long a[] = {289, 1898987, 513, 367, 219};
    long long p[] = {479001599, 23127983, 479001599, 367, 383};
    int res[] = {1, 1, -1, 0, 1};

    int leg_eul, leg_sq_recip;

    time_t start_time = clock();
    for (int i = 0; i < NUMBER_OF_ITERATIONS; i++){
        leg_eul = legendre_euler(a[i % 5], p[i % 5]);
        assert(leg_eul == res[i % 5]);
    }
    time_t end_time = clock();
    double time = (double) (end_time - start_time)/CLOCKS_PER_SEC;
    printf("Legendre-Euler: %f seconds\n", time);

    start_time = clock();
    for (int i = 0; i < NUMBER_OF_ITERATIONS; i++){
        leg_sq_recip = legendre_euclidian(a[i % 5], p[i % 5]);
        assert(leg_sq_recip == res[i % 5]);
    }
    end_time = clock();
    time = (double) (end_time - start_time)/CLOCKS_PER_SEC;
    printf("Legendre quadratic reciprocity and euclidian division: %f seconds\n", time);

    printf("Successful test");
    return 0;

}
