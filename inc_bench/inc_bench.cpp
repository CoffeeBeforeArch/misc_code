// Benchmarks for incrementing an integer using different synchronization
// techniques
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

// Function to incrememnt atomic int
void inc_atomic(std::atomic_int64_t &val) {
  for (int i = 0; i < 100000; i++) val++;
}

// Function to increment using a lock
void inc_mutex(std::mutex &m, long long int &val) {
  for (int i = 0; i < 100000; i++) {
    std::lock_guard<std::mutex> g(m);
    val++;
  }
}

// Atomic bench
static void atomic_bench(benchmark::State &s) {
  // Number of thread
  auto num_threads = s.range(0) - 1;

  // Create an atomic integer (for increments)
  std::atomic_int64_t val{0};

  // Reserve space for threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  // Timing loop
  for (auto _ : s) {
    // Spawn threads
    for (auto i = 0u; i < num_threads; i++)
      threads.emplace_back([&] { inc_atomic(val); });
    inc_atomic(val);

    // Join threads
    for (auto &thread : threads) thread.join();

    // Clear to prevent joining twice
    threads.clear();
  }
}
BENCHMARK(atomic_bench)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

// Lock guard bench
static void lock_guard_bench(benchmark::State &s) {
  // Number of thread
  auto num_threads = s.range(0) - 1;

  // Create an atomic integer (for increments)
  long long int val{0};

  // Reserve space for threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  // Create a mutex
  std::mutex m;

  // Timing loop
  for (auto _ : s) {
    // Spawn threads
    for (auto i = 0u; i < num_threads; i++)
      threads.emplace_back([&] { inc_mutex(m, val); });
    inc_mutex(m, val);

    // Join threads
    for (auto &thread : threads) thread.join();

    // Clear to prevent joining twice
    threads.clear();
  }
}
BENCHMARK(lock_guard_bench)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();
