// Benchmarks for using booleans in arithmetic
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>
#include <vector>

// Benchmark for integrating boolean into multiply and add
// Uses constant that is not a power of 2
static void boolBenchNonPower(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output element
  // Dynamically allocated int isn't optimized away
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  for (auto _ : s) {
    for (auto b : v_in) *sink += 41 * b;
  }

  // Free our memory
  delete sink;
}
BENCHMARK(boolBenchNonPower)->DenseRange(12, 14)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
