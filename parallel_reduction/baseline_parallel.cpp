// Parallel sum reduction w/ statically mapped work
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <random>
#include <thread>
#include <vector>

// Parallel sum reduction with statically mapped work
void reduce_parallel_static(std::vector<std::int64_t> &v_in,
                            std::atomic<std::int64_t> &sum, std::int64_t start,
                            std::int64_t stop) {
  // Keep a local sum to minimize traffic
  std::int64_t local_sum = 0;

  // Sum all the elements for this thread
  for (auto i = start; i < stop; i++) local_sum += v_in[i];

  // Update the global sum
  sum += local_sum;
}

static void parallel_static(benchmark::State &s) {
  // Number of elements to reduce
  std::int64_t N = 1 << s.range(0);

  // Create a vector of input elements
  std::vector<std::int64_t> v_in(N);

  // Set up threads
  std::vector<std::thread> threads;
  auto num_threads = std::thread::hardware_concurrency();
  threads.reserve(num_threads);

  // Number of elements for each thread
  std::int64_t stride = N / num_threads;

  // Main benchmark loop
  for (auto _ : s) {
    // Starting and stopping point for threads
    std::int64_t start = 0;
    std::int64_t stop = 0;

    // Global sum
    std::atomic<std::int64_t> sum{0};

    // Launch threads
    for (unsigned i = 0; i < num_threads - 1; i++) {
      stop = start + stride;
      threads.emplace_back(
          [&] { reduce_parallel_static(v_in, sum, start, stop); });
      start = stop;
    }
    reduce_parallel_static(v_in, sum, start, start + stride);

    // Join threads
    for (auto &t : threads) t.join();

    // Clear threads for next iteration
    threads.clear();
  }
}
BENCHMARK(parallel_static)
    ->DenseRange(20, 25)
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

BENCHMARK_MAIN();
