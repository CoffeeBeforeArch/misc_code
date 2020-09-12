// This program benchmarks a simple spinlock in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <atomic>
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

// Benchmark for naive spinlock
static void naive_spinlock(benchmark::State &s) {
  // Sweep over a range of threads
  auto num_threads = s.range(0);

  // Allocate a vector of threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

}
BENCHMARK(naive_spinlock)
    ->DenseRange(1, std::thread::hardware_concurrency())
    ->UseRealTime();

BENCHMARK_MAIN();
