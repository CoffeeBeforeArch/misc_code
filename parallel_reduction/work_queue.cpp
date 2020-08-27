// Parallel sum reduction w/ dynamically mapped work
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <random>
#include <thread>
#include <vector>

void reduce_parallel_dynamic(std::vector<std::int64_t> &v_in,
                             std::atomic<std::int64_t> &sum,
                             std::atomic<std::int64_t> &pos, std::int64_t N) {
  // Keep a local sum to minimize traffic
  std::int64_t local_sum = 0;

  // Sum all the elements for this thread
  for (auto start = pos.fetch_add(4096); start < N; start = pos.fetch_add(4096))
    for (auto i = start; i < 4096; i++) local_sum += v_in[i];

  // Update the global sum
  sum += local_sum;
}

static void parallel_dynamic(benchmark::State &s) {
  // Number of elements to reduce
  std::int64_t N = 1 << s.range(0);

  // Create a vector of input elements
  std::vector<std::int64_t> v_in(N);

  // Set up threads
  std::vector<std::thread> threads;
  auto num_threads = std::thread::hardware_concurrency();
  threads.reserve(num_threads);

  // Main benchmark loop
  for (auto _ : s) {
    // Global sum and position
    std::atomic<std::int64_t> sum{0};
    std::atomic<std::int64_t> pos{0};

    // Launch threads
    for (unsigned i = 0; i < num_threads - 1; i++) {
      threads.emplace_back([&] { reduce_parallel_dynamic(v_in, sum, pos, N); });
    }
    reduce_parallel_dynamic(v_in, sum, pos, N);

    // Join threads
    for (auto &t : threads) t.join();

    // Clear threads for next iteration
    threads.clear();
  }
}
BENCHMARK(parallel_dynamic)
    ->DenseRange(20, 25)
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

BENCHMARK_MAIN();
