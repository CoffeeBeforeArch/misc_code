// This program implements two dot product implementations in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <cstdlib>
#include <execution>
#include <vector>

// Modern C++ dot product
float dot_product(std::vector<float> &__restrict v1,
                   std::vector<float> &__restrict v2) {
  return std::transform_reduce(std::execution::unseq, begin(v1), end(v1),
                               begin(v2), 0.0);
}

// Benchmark the modern C++ dot product
static void modernDP_double(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  std::vector<float> v1;
  std::fill_n(std::back_inserter(v1), N, rand() % 100);
  std::vector<float> v2;
  std::fill_n(std::back_inserter(v2), N, rand() % 100);

  // Keep our result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product(v1, v2);
  }
}
BENCHMARK(modernDP_double)->DenseRange(8, 10);

// Our benchmark main function
BENCHMARK_MAIN();
