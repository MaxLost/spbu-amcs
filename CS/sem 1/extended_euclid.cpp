#include <iostream>
using namespace std;

// ax + by = GCD(a, b)
//  (0 1)
//  (2 3)   elements id in matrix realization
int extended_euclid(int a, int b, int *x, int *y){
    int matrix[4];
    matrix[0] = matrix[3] = 1;
    matrix[1] = matrix[2] = 0;
    int k = 0;
    while (b > 0){
        int qi = a / b;
        int t = a % b;
        a = b;
        b = t;
        int c = matrix[1], d = matrix[3];
        matrix[1] = matrix[0];
        matrix[3] = matrix[2];
        matrix[0] = qi*matrix[1] + c;
        matrix[2] = qi*matrix[3] + d;
        k++;
        //printf("\n%d %d\n%d %d\n", matrix[0], matrix[1], matrix[2], matrix[3]);
    }
    *x = matrix[3]*(-1);
    *y = matrix[1]*(-1);
    if (!(k & 1))
        *x *= -1;
    else
        *y *= -1;
    return a;
}

int mul_inverse(int x, int m){
    int a, b;
    int g = extended_euclid(x, m, &a, &b);
    if (g != 1)
        return 0;
    else {
        if (a < 0)
            a *= -1;
        a %= m;
        return a;
    }
}
// TEST CASES FOR REVERSE ELEMENT SEARCH
// 84 61 => 8
// 4 11 => 3
// 11 7 => 2
// For extended euclid
// 89 461
/*int main(){
    int a, b, m, x, y;
    cout << "Print 'a' and 'b' for search a 'x' and 'y' to solve the diophantine equation ax + by = GCD(a, b):" << endl;
    cin >> a >> b;
    int gcd = extended_euclid(a, b, &x, &y);
    cout << "x: " << x << endl << "y: " << y << endl;
    if ((a*x + b*y) == gcd)
        cout << "Correct!" << endl;
    else
        cout << "Incorrect" << endl;
    /*
    cout << "Print element and mod for search a reverse element: " << endl;
    cin >> x >> m;
    cout << mul_inverse(x, m);
    return 0;
}
*/
