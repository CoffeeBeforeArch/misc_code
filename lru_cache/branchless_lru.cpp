// Benchmark for an LRU cache w/o branches
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <vector>

// Benchmark for LRU replacement policy
static void branchless_lru_bench(benchmark::State &s) {
  // Number of elements in a set
}
BENCHMARK(branchless_lru_bench);
