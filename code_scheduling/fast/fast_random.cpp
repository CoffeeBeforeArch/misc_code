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

// Our fast modulo operation
static void fastMod(benchmark::State &s) {
  // Number of elements
  int N = s.range(0);

  // Max for mod operator
  int ceil = s.range(1);

  // Vector for input and output
  std::vector<int> input;
  std::vector<int> output;
  input.resize(N);
  output.resize(N);

  // Generate random inputs
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, 255);

  for (auto _ : s) {
    // Generate random numbers but don't profile it
    s.PauseTiming();
    for (int &i : input) {
      i = dist(rng);
    }
    s.ResumeTiming();

    // DON'T compute the mod for each element
    // Skip the expensive operation using a simple compare
    for (int i = 0; i < N; i++) {
      output[i] = (input[i] >= ceil) ? input[i] % ceil : input[i];
    }
  }
}
// Register the benchmark
BENCHMARK(fastMod)->Apply(custom_args);

// Benchmark main function
BENCHMARK_MAIN();
