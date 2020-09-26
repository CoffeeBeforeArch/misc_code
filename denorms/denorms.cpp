// Benchmarks comparing floarting point performance w/ and w/o denormal numbers
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <functional>
#include <random>
#include <vector>

// Benchmark without denormal numbers
static void baseline(benchmark::State &s) {
  // Size of our vectors
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(1.0, 2.0);

  // Create a vector of random doubles
  std::vector<double> v_in(N);
  std::generate(begin(v_in), end(v_in), [&] { return dist(gen); });

  // Create an output vector
  std::vector<double> v_out(N);
  std::generate(begin(v_out), end(v_out), [&] { return 2; });

  // Timing loop
  for (auto _ : s) {
    std::transform(begin(v_in), end(v_in), begin(v_out), begin(v_out),
                   std::plus<double>());
  }
}
BENCHMARK(baseline)->DenseRange(8, 10);

// Benchmark with denormal numbers
static void denorm(benchmark::State &s) {
  // Size of our vectors
  auto N = 1 << s.range(0);

  // Create a vector of random doubles
  std::vector<double> v_in(N);
  std::generate(begin(v_in), end(v_in), [&] { return 0x1p-1020; });

  // Create an output vector
  std::vector<double> v_out(N);
  std::generate(begin(v_out), end(v_out), [&] { return 2; });

  // Timing loop
  for (auto _ : s) {
    std::transform(begin(v_in), end(v_in), begin(v_out), begin(v_out),
                   std::plus<double>());
  }
}
BENCHMARK(denorm)->DenseRange(8, 10);

BENCHMARK_MAIN();
