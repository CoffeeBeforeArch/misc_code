// Benchmark for removing duplicates from a vector
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <unordered_set>
#include <vector>

// Function for generating argument pairs
static void custom_args(benchmark::internal::Benchmark *b) {
  for (auto i : {10, 11, 12}) {
    for (auto j : {10, 100, 1000, 10000}) {
      b = b->ArgPair(i, j);
    }
  }
}

// Baseline benchmark used by sorting vectors
static void baseline(benchmark::State &s) {
  // Create input and output vectors
  int N = 1 << s.range(0);
  std::vector<int> v_in(N);
  std::vector<int> v_out;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, s.range(1));

  // Fill the input vector with random numbers
  std::generate(begin(v_in), end(v_in), [&] { return dist(rng); });

  // Benchmark loop
  for (auto _ : s) {
    // For every value in the input vector
    for (auto i : v_in) {
      // Check if it is not in the output vector
      if (std::find(begin(v_out), end(v_out), i) == v_out.end())
        // And put it in if it's not
        v_out.push_back(i);
    }

    // Clear each iteration
    v_out.clear();
  }
}
BENCHMARK(baseline)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

// Benchmark that filters the values in a hash set
static void hash_set(benchmark::State &s) {
  // Create input and output vectors
  int N = 1 << s.range(0);
  std::vector<int> v_in(N);
  std::unordered_set<int> filter;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, s.range(1));

  // Fill the input vector with random numbers
  std::generate(begin(v_in), end(v_in), [&] { return dist(rng); });

  // Benchmark loop
  for (auto _ : s) {
    // Insert each element into the unordered set
    // Duplicate will be overridden
    for (auto i : v_in) filter.insert(i);

    // Clear each iteration
    filter.clear();
  }
}
BENCHMARK(hash_set)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

// Benchmark that filters with a has set then copies into a vector
static void hash_set_copy(benchmark::State &s) {
  // Create input and output vectors
  int N = 1 << s.range(0);
  std::vector<int> v_in(N);
  std::vector<int> v_out;
  std::unordered_set<int> filter;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, s.range(1));

  // Fill the input vector with random numbers
  std::generate(begin(v_in), end(v_in), [&] { return dist(rng); });

  // Benchmark loop
  for (auto _ : s) {
    // Create the output vector and filter inside the loop so it gets cleared
    // each iteration

    // Insert each element into the unordered set
    // Duplicate will be overridden
    for (auto i : v_in) filter.insert(i);
    for (auto i : filter) v_out.push_back(i);

    // Clear each iteration
    v_out.clear();
    filter.clear();
  }
}
BENCHMARK(hash_set_copy)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
