#include <iostream>
using namespace std;

int binary_euclid(int a, int b){
    int k = 0;
    // Check on trivial input values
    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;
    // Getting out common 2's
    while ((a & 1) == 0 && (b & 1) == 0){
        a = a >> 1;
        b = b >> 1;
        k++;
    }
    // Getting out 2's from even number (other number is odd)
    while (a & 1 == 0)
        a = a >> 1;
    while (b & 1 == 0)
        b = b >> 1;
    // Decreasing numbers for GCD to MIN. Because GCD(a, b) = GCD(a - b, b) if a & b is odd
    while (a > 1 && b > 1){
        if (b > a)
            swap(a, b);
        a = a - b;
        while (a & 1 == 0) // Check on even number and getting out 2 from it
            a = a >> 1;
    }
    // Choosing correct value for answer and putting it into A
    if (a != 0 && b != 0){
        if (b < a)
            a = b;
    }
    else if (a == 0)
        a = b;
    // Recovering 2^k
    a = a << k;
    return a;
}

int euclid(int a, int b){
    while (a % b != 0){
        if (b > a)
            swap(a, b);
        a = a % b;
    }
    return b;
}

int rec_euclid(int a, int b){
    if (a % b == 0)
        return b;
    if (b > a)
        swap(a, b);
    a = a % b;
    return rec_euclid(a, b);
}

int main(){
    int a, b;
    cin >> a >> b;
    cout << euclid(a, b) << endl;
    cout << binary_euclid(a, b) << endl;
    return 0;
}
