// This program shows off a neat optimization for fast a faster
// modulo operation in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <random>
#include <vector>

// Function for generating argument pairs
static void custom_args(benchmark::internal::Benchmark *b) {
  for (int i = 1 << 4; i <= 1 << 10; i <<= 2) {
    // Collect stats at 1/8, 1/2, and 7/8
    for (int j : {32, 128, 224}) {
      b = b->ArgPair(i, j);
    }
  }
}

// Baseline for intuitive modulo operation
static void fastModHint(benchmark::State &s) {
  // Number of elements
  int N = s.range(0);

  // Max for mod operator
  int ceil = s.range(1);

  // Vector for input and output of modulo
  std::vector<int> input;
  std::vector<int> output;
  input.resize(N);
  output.resize(N);

  // Generate random inputs
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 255);
  for (int &i : input) {
    i = dist(rng);
  }

  for (auto _ : s) {
    // DON'T compute the mod for each element
    // Skip the expensive operation using a simple compare
    for (int i = 0; i < N; i++) {
      // Hint to the compiler that we usually skip the mod
      output[i] =
          __builtin_expect(input[i] >= ceil, 0) ? input[i] % ceil : input[i];
    }
  }
}
// Register the benchmark
BENCHMARK(fastModHint)->Apply(custom_args);

// Benchmark main function
BENCHMARK_MAIN();
