// Short program for printing out sizing information
// By: Nick from CoffeeBeforeArch

#include <cstdlib>
#include <iostream>
#include <vector>

// Overloaded new operator to track dynamic allocation
void* operator new(size_t N) {
  std::cout << "Allocating " << N << " bytes of memory!\n";
  return malloc(N);
}

int main() {
  // Measure for the largest size
  const size_t N = 1 << 12;

  // Create the vectors used in the benchmarks
  std::vector<bool> v_bool(N);
  std::vector<char> v_char(N);
  std::vector<int> v_int(N);

  return 0;
}
