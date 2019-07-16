// This program takes the power of different numbers and is compiled
// using CMake
// By: Nick from CoffeeBeforeArch

#include <iostream>
#include <cmath>

using namespace std;

int main(){
    // Compute some a power calculation
    int base = 4;
    int exponent = 3;
    int result = pow(base, exponent);

    // Print the result
    cout << base << "^" << exponent << " = " << result << endl;
    return 0;
}
