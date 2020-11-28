// This example shows how hardware barriers for x86
// By: Nick from CoffeeBeforeArch

#include <atomic>
#include <cstdio>
#include <thread>

void reorder(std::atomic<int> &signal, int &v1, int &v2, int &rec) {
  while (true) {
    while (!signal.load())
      ;

    // Write to v2
    v2 = 1;

    // Barrier to prevent re-ordering of read and write by compiler
    //asm volatile("" : : : "memory");
    asm volatile("mfence" : : : "memory");
    
    // Read v1
    rec = v1;

    // Say we're done for this iterations
    signal.store(false);
  }
}

int main() {
  // Signaling atomics
  alignas(64) std::atomic<int> a{0};
  alignas(64) std::atomic<int> b{0};

  // Variable for memory re-ordering
  alignas(64) int v1, v2;
  alignas(64) int r1, r2;

  // Start threads
  std::thread t1([&] { reorder(a, v1, v2, r1); });
  std::thread t2([&] { reorder(b, v2, v1, r2); });

  // Go forever
  for (int i = 0; i < 1000000; i++) {
    // Initialize the shared variables
    v1 = 0;
    v2 = 0;

    // Tell the threads to go
    a.store(1);
    b.store(1);

    // Wait for the threads to finish
    while (a.load() == 1 && b.load() == 1)
      ;

    if (r1 == 0 && r2 == 0) {
      printf("SAW MEMORY RE-ORDERING ON ITERATION %d\n", i);
    }
  }

  return 0;
}
