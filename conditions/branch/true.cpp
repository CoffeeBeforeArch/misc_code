// Benchmarks using branches for conditionally adding a value
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <vector>

// Benchmark for using branches
static void branchBenchTrue(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), []() { return true; });

  // Output element
  // Dynamically allocated int isn't optimized away
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  for (auto _ : s) {
    for (auto b : v_in)
      if (b) *sink += 41;
  }

  // Free our memory
  delete sink;
}
BENCHMARK(branchBenchTrue)->DenseRange(10, 12);

BENCHMARK_MAIN();
