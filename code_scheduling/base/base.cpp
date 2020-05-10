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

// Baseline benchmark
static void baseMod(benchmark::State &s) {
  // Number of elements in the vectors
  int N = s.range(0);

  // Max for mod operator
  int ceil = s.range(1);

  // Vector for input and output of modulo
  std::vector<int> input;
  std::vector<int> output;
  input.resize(N);
  output.resize(N);

  // Generate random inputs (uniform random dist. between 0 & 255)
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 255);
  for (int &i : input) {
    i = dist(rng);
  }

  // Main benchmark loop
  while (s.KeepRunning()) {
    // Compute the modulo for each element
    for (int i = 0; i < N; i++) {
      output[i] = input[i] % ceil;
    }
  }
}
// Register the benchmark
BENCHMARK(baseMod)->Apply(custom_args);

// Benchmark main function
BENCHMARK_MAIN();
