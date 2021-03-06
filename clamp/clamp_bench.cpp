// Benchmarks for compiler optimizations of a clamp function
// By: Nick from CoffeeBeforeArch

#include <algorithm>
#include <random>
#include <vector>

#include "benchmark/benchmark.h"

// Benchmark for a clamp function
// std::vector + for-loop
static void clamp_bench(benchmark::State &s) {
  // Number of elements in the vector
  auto N = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 1024);

  // Create a vector of random integers
  std::vector<int> v_in(N);
  std::vector<int> v_out(N);
  std::generate(begin(v_in), end(v_in), [&]() { return dist(rng); });

  // Main benchmark loop
  for (auto _ : s) {
    for (int i = 0; i < N; i++) {
      v_out[i] = (v_in[i] > 512) ? 512 : v_in[i];
    }
  }
}
BENCHMARK(clamp_bench)->DenseRange(8, 10);

// Benchmark for a clamp function
// Raw pointers + for-loop
static void clamp_bench_raw_ptr(benchmark::State &s) {
  // Number of elements in the vector
  auto N = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 1024);

  // Create a vector of random integers
  int *v_in = new int[N]();
  int *v_out = new int[N]();
  std::generate(v_in, v_in + N, [&]() { return dist(rng); });

  // Main benchmark loop
  for (auto _ : s) {
    for (int i = 0; i < N; i++) {
      v_out[i] = (v_in[i] > 512) ? 512 : v_in[i];
    }
  }

  delete[] v_in;
  delete[] v_out;
}
BENCHMARK(clamp_bench_raw_ptr)->DenseRange(8, 10);

// Benchmark for a clamp function
// std::vector + std::transform
static void clamp_bench_lambda(benchmark::State &s) {
  // Number of elements in the vector
  auto N = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 1024);

  // Create a vector of random integers
  std::vector<int> v_in(N);
  std::vector<int> v_out(N);
  std::generate(begin(v_in), end(v_in), [&]() { return dist(rng); });

  // Our clamp function
  auto clamp = [](int in) { return (in > 512) ? 512 : in; };

  // Main benchmark loop
  for (auto _ : s) {
    std::transform(begin(v_in), end(v_in), begin(v_out), clamp);
  }
}
BENCHMARK(clamp_bench_lambda)->DenseRange(8, 10);

// Benchmark for a clamp function
// Raw pointers + std::transform
static void clamp_bench_raw_ptr_lambda(benchmark::State &s) {
  // Number of elements in the vector
  auto N = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 1024);

  // Create a vector of random integers
  int *v_in = new int[N]();
  int *v_out = new int[N]();
  std::generate(v_in, v_in + N, [&]() { return dist(rng); });

  // Our clamp function
  auto clamp = [](int in) { return (in > 512) ? 512 : in; };

  // Main benchmark loop
  for (auto _ : s) {
    std::transform(v_in, v_in + N, v_out, clamp);
  }

  delete[] v_in;
  delete[] v_out;
}
BENCHMARK(clamp_bench_raw_ptr_lambda)->DenseRange(8, 10);

BENCHMARK_MAIN();
