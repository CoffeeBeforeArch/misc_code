// A simple example of double buffering in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <cstdlib>
#include <thread>
#include <vector>

// Benchmark to fill the vector
static void baseline_fill(benchmark::State &s) {
  // Number of elements
  auto n = s.range(0);

  // Create our vector
  std::vector<int> v(n);

  // Benchmark loop
  for (auto _ : s) {
    // For 10 iterations
    for (int i = 0; i < 10; i++)
      // Fill the vector with random numbers
      for (auto &val : v) val = rand() % 100;
  }
}
BENCHMARK(baseline_fill)->DenseRange(10, 12)->Unit(benchmark::kMicrosecond);

// Work function for our benchmark
void work(std::vector<int> &v) {
  // Divisor
  auto div = rand() % 27 + 1;

  // Kill some time doing division
  for (auto &val : v) val /= div;
  for (auto &val : v) val /= div;
  for (auto &val : v) val /= div;
  for (auto &val : v) val /= div;
}

// Benchmark to use the data in the vector
static void baseline_drain(benchmark::State &s) {
  // Number of elements
  auto n = s.range(0);

  // Create our vector
  std::vector<int> v(n);

  // Benchmark loop
  for (auto _ : s) {
    // For 10 iterations
    for (int i = 0; i < 10; i++) {
      // Fill the vector with random numbers
      work(v);
    }
  }
}
BENCHMARK(baseline_drain)->DenseRange(10, 12)->Unit(benchmark::kMicrosecond);

// Benchmark to fill and drain the vector serially
static void baseline_serial(benchmark::State &s) {
  // Number of elements
  auto n = s.range(0);

  // Create our vector
  std::vector<int> v(n);

  // Benchmark loop
  for (auto _ : s) {
    // For 10 iterations
    for (int i = 0; i < 10; i++) {
      // Fill the vector with random numbers
      for (auto &val : v) val = rand() % 100;
      
      // Process that vector
      work(v);
    }
  }
}
BENCHMARK(baseline_serial)->DenseRange(10, 12)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
