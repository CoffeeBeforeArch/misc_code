// Benchmark for the components of our filtering technique
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

// Function for generating argument pairs
static void custom_args(benchmark::internal::Benchmark *b) {
  for (auto i : {14, 15, 16}) {
    for (auto j : {10, 100, 1000, 10000}) {
      b = b->ArgPair(i, j);
    }
  }
}

// Benchmark that filters the values in a hash set
static void filter(benchmark::State &s) {
  // Create input and output vectors
  int N = 1 << s.range(0);
  std::vector<int> v_in(N);
  std::unordered_map<int, int> filter;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, s.range(1));

  // Fill the input vector with random numbers
  std::generate(begin(v_in), end(v_in), [&] { return dist(rng); });

  // Benchmark loop
  for (auto _ : s) {
    // A vector for our sorted non-duplicates
    std::vector<int> tmp;
    // Go through each element
    for (auto i : v_in) {
      // If it is in the filter, increment the number of instances
      if (filter.contains(i))
        filter[i]++;
      else {
        // Otherwise, we found the first one
        filter[i] = 1;
        // Save one unique value to the vector
        tmp.push_back(i);
      }
    }

    // Clear each iteration
    filter.clear();
  }
}
BENCHMARK(filter)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

// Benchmark that filters the values in a hash set
static void sort(benchmark::State &s) {
  // Create input and output vectors
  int N = 1 << s.range(0);
  std::vector<int> v_in(N);
  std::unordered_map<int, int> filter;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<int> dist(0, s.range(1));

  // Fill the input vector with random numbers
  std::generate(begin(v_in), end(v_in), [&] { return dist(rng); });

  // A vector for our sorted non-duplicates
  std::vector<int> filtered;
  // Go through each element
  for (auto i : v_in) {
    // If it is in the filter, increment the number of instances
    if (filter.contains(i))
      filter[i]++;
    else {
      // Otherwise, we found the first one
      filter[i] = 1;
      // Save one unique value to the vector
      filtered.push_back(i);
    }
  }

  // Benchmark loop
  for (auto _ : s) {
    // Refresh our vector each iteration
    std::vector<int> tmp = filtered;

    // Sort the non-duplicates
    std::ranges::sort(tmp);
  }
}
BENCHMARK(sort)->Apply(custom_args)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
