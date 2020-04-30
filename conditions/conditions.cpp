// Benchmarks comparing branches and building in conditions to computation
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <execution>
#include <functional>
#include <numeric>
#include <vector>

// Benchmark for using branches
static void branchBench(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), []() { return rand() % 2; });
  
  // Output vector
  std::vector<int> v_out(N);

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (size_t i = 0; i < v_in.size(); ++i)
      if (v_in[i]) v_out[i] += 41;
  }
}
BENCHMARK(branchBench)->DenseRange(10, 12);

// Benchmark for integrating boolean into multiply and add
static void logicBenchBool(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create a vector of random booleans
  std::vector<bool> v_in(N);
  std::generate(begin(v_in), end(v_in), []() { return rand() % 2; });
  
  // Output vector
  std::vector<int> v_out(N);

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (size_t i = 0; i < v_in.size(); ++i) v_out[i] += 41 * v_in[i];
  }
}
BENCHMARK(logicBenchBool)->DenseRange(10, 12);

// Benchmark for integrating character into multiply and add
static void logicBenchChar(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create a vector of random booleans stored as characters
  std::vector<char> v_in(N);
  std::generate(begin(v_in), end(v_in), []() { return rand() % 2; });
  
  // Output vector
  std::vector<int> v_out(N);

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (size_t i = 0; i < v_in.size(); ++i) v_out[i] += 41 * v_in[i];
  }
}
BENCHMARK(logicBenchChar)->DenseRange(10, 12);

// Benchmark for integrating integer into multiply and add
static void logicBenchInt(benchmark::State &s) {
  // Get the input vector size
  auto N = 1 << s.range(0);

  // Create a vector of random booleans stored as integers
  std::vector<int> v_in(N);
  std::generate(begin(v_in), end(v_in), []() { return rand() % 2; });
  
  // Output vector
  std::vector<int> v_out(N);

  // Benchmark main loop
  while (s.KeepRunning()) {
    for (size_t i = 0; i < v_in.size(); ++i) v_out[i] += 41 * v_in[i];
  }
}
BENCHMARK(logicBenchInt)->DenseRange(10, 12);

BENCHMARK_MAIN();
