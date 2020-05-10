// This program implements a baseline dot product in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdlib>
#include <vector>

// Classic C-style dot product
float dot_product(std::vector<float> &__restrict v1,
                  std::vector<float> &__restrict v2) {
  float tmp = 0.0;
  for (size_t i = 0; i < v1.size(); i++) {
    tmp += v1[i] * v2[i];
  }
  return tmp;
}

// Benchmark the baseline C-style dot product
static void baseDP(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  std::vector<float> v1;
  std::fill_n(std::back_inserter(v1), N, rand() % 100);
  std::vector<float> v2;
  std::fill_n(std::back_inserter(v2), N, rand() % 100);

  // Keep the result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product(v1, v2);
  }
}
BENCHMARK(baseDP)->DenseRange(8, 10);

// Our benchmark main function
BENCHMARK_MAIN();
