// Benchmark of atomic ref vs an atomic
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <atomic>
#include <thread>

// Benchmark for using atomic
static void atomic_bench(benchmark::State &s) {
  // Set the number of iterations create an atomic int
  std::size_t N = 1 << 10;
  std::atomic_int64_t val{0};

  // Our lambda for multiple threads
  auto work = [&]() {
    // Increment the non-atomic int atomically!
    for (std::size_t i = 0; i < N; i++) {
      val++;
    }
  };

  // Benchmark Loop
  for (auto _ : s) {
    // Create some threads
    std::thread t0(work);
    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    // Join the threads
    t0.join();
    t1.join();
    t2.join();
    t3.join();

    // Now work
    work();
  }
}
BENCHMARK(atomic_bench);

// Benchmark for using atomic ref
static void atomic_ref_bench(benchmark::State &s) {
  // Create an integer and a atomic_ref to the integer
  // Set the number of iterations create an atomic reference
  std::size_t N = 1 << 10;
  std::int64_t val = 0;
  std::atomic_ref<std::int64_t> ref(val);

  // Our lambda for multiple threads
  auto work = [&]() {
    // Increment the non-atomic int atomically!
    for (std::size_t i = 0; i < N; i++) {
      ref++;
    }
  };

  // Our lambda for a single thread
  auto work2 = [&]() {
    // Increment the non-atomic int atomically!
    for (std::size_t i = 0; i < N; i++) {
      val++;
    }
  };

  // Benchmark Loop
  for (auto _ : s) {
    // Create some threads
    std::thread t0(work);
    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    // Join the threads
    t0.join();
    t1.join();
    t2.join();
    t3.join();

    // Now work
    work2();
  }
}
BENCHMARK(atomic_ref_bench);

BENCHMARK_MAIN();
