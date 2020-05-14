// Benchmark filling a vector with different numbers
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <vector>

// Store 0
static void store_bench_0(benchmark::State &s) {
  // Vector of 2^N integers
  std::vector<int> v(1 << s.range(0));

  // Benchmarking filling the vector with zeros
  for (auto _ : s) {
    std::fill(begin(v), end(v), 0);
  }
}
BENCHMARK(store_bench_0)->DenseRange(20, 22)->Unit(benchmark::kMillisecond);

// Store 1
static void store_bench_1(benchmark::State &s) {
  // Vector of 2^N integers
  std::vector<int> v(1 << s.range(0));

  // Benchmarking filling the vector with non-zero
  for (auto _ : s) {
    std::fill(begin(v), end(v), 1);
  }
}
BENCHMARK(store_bench_1)->DenseRange(20, 22)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
