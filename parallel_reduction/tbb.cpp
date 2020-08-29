// Parallel sum reduction w/ parallel STL (TBB)
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdint>
#include <execution>
#include <random>
#include <vector>

static void parallel_tbb(benchmark::State &s) {
  // Number of elements to reduce
  std::int64_t N = 1 << s.range(0);

  // Create a vector of input elements
  std::vector<std::int64_t> v_in(N);

  // Sink variable for storing the results
  std::int64_t sink = 0;

  // Main benchmark loop
  for (auto _ : s) {
    // Mark as `DoNotOptimize` to make sure the result isn't optimized out
    benchmark::DoNotOptimize(
        sink = std::reduce(std::execution::par_unseq, begin(v_in), end(v_in)));
  }
}
BENCHMARK(parallel_tbb)
    ->DenseRange(20, 25)
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

BENCHMARK_MAIN();
