// A simple benchmark for absolute value
// By Nick from CoffeeBeforeArch

#include <vector>
#include <benchmark/benchmark.h>
#include <random>
#include <cstdlib>

// A simple benchmark for absolute value
static void abs_bench_base(benchmark::State &s) {
  // Number of elements
  auto num_elements = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-1, 1);
  
  // Fill our vector with random numbers
  std::vector<float> v_in(num_elements);
  std::generate(begin(v_in), end(v_in), [&]() { return dist(rng); });

  // Create a vector for results
  std::vector<float> v_out(num_elements);
  
  // Do our absolute value
  for (auto _ : s) {
    for (int i = 0; i < v_in.size(); i++) {
      v_out[i] = v_in[i] > 0 ? v_in[i] : - v_in[i];
    }
  }
}
BENCHMARK(abs_bench_base)->DenseRange(6, 12);

// A simple benchmark for absolute value
static void abs_bench_branch(benchmark::State &s) {
  // Number of elements
  auto num_elements = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-1, 1);
  
  // Fill our vector with random numbers
  std::vector<float> v_in(num_elements);
  std::generate(begin(v_in), end(v_in), [&]() { return dist(rng); });

  // Create a vector for results
  std::vector<float> v_out(num_elements);
  
  // Do our absolute value
  for (auto _ : s) {
    for (int i = 0; i < v_in.size(); i++) {
      if (v_in[i] > 0) v_out[i] =  v_in[i];
      else v_out[i] = - v_in[i];
    }
  }
}
BENCHMARK(abs_bench_branch)->DenseRange(6, 12);


// A simple benchmark for absolute value
static void abs_bench_std(benchmark::State &s) {
  // Number of elements
  auto num_elements = 1 << s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-1, 1);
  
  // Fill our vector with random numbers
  std::vector<float> v_in(num_elements);
  std::generate(begin(v_in), end(v_in), [&]() { return dist(rng); });

  // Create a vector for results
  std::vector<float> v_out(num_elements);
  
  // Do our absolute value
  for (auto _ : s) {
    for (int i = 0; i < v_in.size(); i++) {
      v_out[i] = std::abs(v_in[i]);
    }
  }
}
BENCHMARK(abs_bench_std)->DenseRange(6, 12);


BENCHMARK_MAIN();
