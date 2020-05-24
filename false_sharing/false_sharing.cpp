// This program shows off false sharing in C++ using std::atomic and std::thread
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <atomic>
#include <thread>

// Simple function for incrememnting an atomic int
void work(std::atomic<int>& a) {
  for (int i = 0; i < 100000; i++) {
    a++;
  }
}

// Helper function for falseSharing benchmark
void false_sharing() {
  std::atomic<int> a{0};
  std::atomic<int> b{0};
  std::atomic<int> c{0};
  std::atomic<int> d{0};

  // Create four threads and use lambda to launch work
  std::thread t0([&]() { work(a); });
  std::thread t1([&]() { work(b); });
  std::thread t2([&]() { work(c); });
  std::thread t3([&]() { work(d); });

  // Join the threads
  t0.join();
  t1.join();
  t2.join();
  t3.join();
}

// False sharing benchmark
static void falseSharing(benchmark::State& s) {
  while (s.KeepRunning()) {
    false_sharing();
  }
}
BENCHMARK(falseSharing)->UseRealTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
