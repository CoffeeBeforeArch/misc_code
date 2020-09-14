// This program benchmarks a simple spinlock in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <atomic>
#include <cstdint>
#include <thread>

// Simple Spinlock
struct Spinlock {
  // Lock is just an atomic bool
  std::atomic<bool> locked{false};

  // Locking mechanism
  void lock() {
    // Exchange will return the previous value of the lock
    // If the lock is free (false), it is set to true and the loop exits
    // If the lock is taken (true), spin in the loop until the loop exits
    while (locked.exchange(true))
      ;
  }

  // Unlocking mechanism
  // Just set the lock to free (0)
  // Can also use the assignment operator
  void unlock() { locked.store(false); }
};

// Increment val once each time the lock is acquired
void inc_small(Spinlock &s, std::int64_t &val) {
  for (int i = 0; i < 100000; i++) {
    s.lock();
    val++;
    s.unlock();
  }
}

// Increment val 100 times each time the lock is acquired
void inc_medium(Spinlock &s, std::int64_t &val) {
  for (int i = 0; i < 1000; i++) {
    s.lock();
    for (int j = 0; j < 100; j++) val++;
    s.unlock();
  }
}

// Increment val 1000 times each time the lock is acquired
void inc_large(Spinlock &s, std::int64_t &val) {
  for (int i = 0; i < 100; i++) {
    s.lock();
    for (int j = 0; j < 1000; j++) val++;
    s.unlock();
  }
}

// Benchmark for naive spinlock
static void naive_small(benchmark::State &s) {
  // Sweep over a range of threads
  auto num_threads = s.range(0);

  // Value we will increment
  std::int64_t val = 0;

  // Allocate a vector of threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  Spinlock sl;

  // Timing loop
  for (auto _ : s) {
    for (auto i = 0u; i < num_threads; i++) {
      threads.emplace_back([&] { inc_small(sl, val); });
    }
    // Join threads
    for (auto &thread : threads) thread.join();
    threads.clear();
  }
}
BENCHMARK(naive_small)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

// Benchmark for naive spinlock
static void naive_medium(benchmark::State &s) {
  // Sweep over a range of threads
  auto num_threads = s.range(0);

  // Value we will increment
  std::int64_t val = 0;

  // Allocate a vector of threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  Spinlock sl;

  // Timing loop
  for (auto _ : s) {
    for (auto i = 0u; i < num_threads; i++) {
      threads.emplace_back([&] { inc_medium(sl, val); });
    }
    // Join threads
    for (auto &thread : threads) thread.join();
    threads.clear();
  }
}
BENCHMARK(naive_medium)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->UseRealTime()
    ->Unit(benchmark::kMicrosecond);

// Benchmark for naive spinlock
static void naive_large(benchmark::State &s) {
  // Sweep over a range of threads
  auto num_threads = s.range(0);

  // Value we will increment
  std::int64_t val = 0;

  // Allocate a vector of threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  Spinlock sl;

  // Timing loop
  for (auto _ : s) {
    for (auto i = 0u; i < num_threads; i++) {
      threads.emplace_back([&] { inc_large(sl, val); });
    }
    // Join threads
    for (auto &thread : threads) thread.join();
    threads.clear();
  }
}
BENCHMARK(naive_large)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->UseRealTime()
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
