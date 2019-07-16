// This file contains the definition of an exponent function
int pow(int base, int exponent){
    int tmp = base;
    
    // Return 1 for exponent of 0
    if (exponent == 0){
        return 1;
    }

    // Accumulate exponent through multiplication
    for(int i = 0; i < exponent - 1; i++){
        tmp *= base;
    }

    return tmp;
}
