// Benchmarks for using chars to store boolean values
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>
#include <vector>

// Benchmark for integrating character into multiply and add
// Uses value known only at runtime
static void charBenchInput(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<char> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output element
  // Dynamically allocated int isn't optimized away
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  for (auto _ : s) {
    for (auto b : v_in) *sink += s.range(0) * b;
  }

  // Free our memory
  delete sink;
}
BENCHMARK(charBenchInput)->DenseRange(12, 14)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
