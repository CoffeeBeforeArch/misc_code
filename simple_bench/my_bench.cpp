// Benchmark std::accumulate

#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <vector>

#include "benchmark/benchmark.h"

static void accumulate_bench(benchmark::State &s) {
  // Number of elements (2^10)
  auto N = 1 << s.range(0);

  // Create a vector of random numbers
  std::vector<int> v(N);
  std::generate(begin(v), end(v), [] { return rand() % 100; });

  // Variable for our results
  int result = 0;

  // Main timing loop
  for (auto _ : s) {
    benchmark::DoNotOptimize(result = std::accumulate(begin(v), end(v), 0));
  }
}
BENCHMARK(accumulate_bench)->DenseRange(20, 22)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
