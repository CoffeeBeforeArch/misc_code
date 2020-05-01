// Benchmarks comparing branches and building in conditions to computation
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <cstdint>
#include <execution>
#include <functional>
#include <numeric>
#include <vector>
#include <random>

// Benchmark for using branches
static void branchBench(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output vector
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (auto b : v_in)
      if (b) *sink += 41;
  }

  delete sink;
}
BENCHMARK(branchBench)->DenseRange(10, 12);

// Benchmark for integrating boolean into multiply and add
static void logicBenchBool(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output vector
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (auto b : v_in) *sink += 41 * b;
  }

  delete sink;
}
BENCHMARK(logicBenchBool)->DenseRange(10, 12);

// Benchmark for integrating character into multiply and add
static void logicBenchChar(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<char> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output vector
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (auto b : v_in) *sink += 41 * b;
  }

  delete sink;
}
BENCHMARK(logicBenchChar)->DenseRange(10, 12);

// Benchmark for integrating integer into multiply and add
static void logicBenchInt(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.5);

  // Create a vector of random booleans
  std::vector<int> v_in(N);
  std::generate(begin(v_in), end(v_in), [&]() { return d(gen); });

  // Output vector
  int *sink = new int;
  *sink = 0;

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (auto b : v_in) *sink += 41 * b;
  }

  delete sink;
}
BENCHMARK(logicBenchInt)->DenseRange(10, 12);

BENCHMARK_MAIN();
