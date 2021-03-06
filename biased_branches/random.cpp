// Benchmarks using branches for conditionally adding a value
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <vector>

// Function for generating argument pairs
static void custom_args(benchmark::internal::Benchmark *b) {
  for (auto i : {14}) {
    for (auto j : {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}) {
      b = b->ArgPair(i, j);
    }
  }
}

// Benchmark for using branches
static void branchBenchRandom(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Get the distribution
  double probability = s.range(1) / 100.0;

  // Create random number generator
  // Bernoulli distribution gives T/F outcomes
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output element
  int sink = 0;

  // Benchmark main loop
  for (auto _ : s) {
    for (auto b : v_in)
      if (b) benchmark::DoNotOptimize(sink += s.range(0));
  }
}
BENCHMARK(branchBenchRandom)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
