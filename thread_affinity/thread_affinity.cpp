// This program shows off setting thread affinity
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <pthread.h>

#include <atomic>
#include <cassert>
#include <thread>

// Simple function for incrememnting an atomic int
void work(std::atomic<int>& a) {
  for (int i = 0; i < 100000; i++) {
    a++;
  }
}

//  Aligned type
//  Allows us to keep to atomics from sitting on the same cache line
struct alignas(64) AlignedAtomic {
  AlignedAtomic(int v) { val = v; }
  std::atomic<int> val;
};

void os_scheduler() {
  AlignedAtomic a{0};
  AlignedAtomic b{0};

  // Create four threads and use lambda to launch work
  std::thread t1([&]() { work(a.val); });
  std::thread t2([&]() { work(a.val); });
  std::thread t3([&]() { work(b.val); });
  std::thread t4([&]() { work(b.val); });

  // Join the threads
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}

// Data sharing benchmark w/ OS scheduling
static void osScheduling(benchmark::State& s) {
  while (s.KeepRunning()) {
    os_scheduler();
  }
}
BENCHMARK(osScheduling)->UseRealTime()->Unit(benchmark::kMillisecond);

void thread_affinity() {
  AlignedAtomic a{0};
  AlignedAtomic b{0};

  // Create cpu sets for threads 0,1 and 2,3
  cpu_set_t cpu_set_1;
  cpu_set_t cpu_set_2;

  // Zero them out
  CPU_ZERO(&cpu_set_1);
  CPU_ZERO(&cpu_set_2);

  // And set the CPU cores we want to pin the threads too
  CPU_SET(0, &cpu_set_1);
  CPU_SET(1, &cpu_set_2);

  // Create thread 0 and 1, and pin them to core 0
  std::thread t0([&]() { work(a.val); });
  assert(pthread_setaffinity_np(t0.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);
  std::thread t1([&]() { work(a.val); });
  assert(pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);
 
  // Create thread 1 and 2, and pin them to core 1
  std::thread t2([&]() { work(b.val); });
  assert(pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_2) == 0);
  std::thread t3([&]() { work(b.val); });
  assert(pthread_setaffinity_np(t3.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_2) == 0);

  // Join the threads
  t0.join();
  t1.join();
  t2.join();
  t3.join();
}

// Data sharing benchmark w/ manual scheduling
static void threadAffinity(benchmark::State& s) {
  while (s.KeepRunning()) {
    thread_affinity();
  }
}
BENCHMARK(threadAffinity)->UseRealTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
