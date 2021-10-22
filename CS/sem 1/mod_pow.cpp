#include <iostream>
using namespace std;

// x^y mod m
long long mod_pow_rec(long long x, long long y, long long m){
    if (y == 0)
        return 1 % m;
    if (y == 1)
        return x % m;
    else {
        long long lastx = x;
        long long x1 = (x * x) % m;
        x = (mod_pow_rec(x1, y >> 1, m)) % m;
        if (y & 1 == 1)
            x *= lastx;
    }
    return x % m;
}

long long mod_pow(long long x, long long y, long long m){
    long long powi = x % m;
    long long ans = 1;
    while (y){
        if (y & 1){
            ans *= powi;
        }
        powi = (powi * powi) % m;
        y >>= 1;
    }
    return ans % m;
}

int main(){
    long x, y, m;
    cin >> x >> y >> m;
    cout << mod_pow1(x, y, m) << endl;
    cout << mod_pow2(x, y, m) << endl;
    return 0;
}

