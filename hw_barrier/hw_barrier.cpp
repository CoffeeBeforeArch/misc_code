// This example shows how hardware barriers for x86
// By: Nick from CoffeeBeforeArch

#include <semaphore.h>

#include <atomic>
#include <cassert>
#include <cstdio>
#include <thread>

void reorder(sem_t &start, sem_t &end, int &v1, int &v2, int &rec) {
  // Keep going forever
  while (true) {
    // Wait for the signal to start
    sem_wait(&start);

    // Write to v2
    v1 = 1;

    // Barrier to prevent re-ordering of read and write by compiler
    // asm volatile("" : : : "memory");

    // Barrier to prevent re-ordering in the hardware!
    asm volatile("mfence" : : : "memory");

    // Read v1
    rec = v2;

    // Say we're done for this iteration
    sem_post(&end);
  }
}

int main() {
  // Semaphores for signaling threads
  sem_t start1;
  sem_t start2;
  sem_t end;
  sem_init(&start1, 0, 0);
  sem_init(&start2, 0, 0);
  sem_init(&end, 0, 0);

  // Variable for memory re-ordering
  // Use alignas to put on different cache lines
  alignas(64) int v1 = 0, v2 = 0;
  alignas(64) int r1 = 0, r2 = 0;

  // Start threads
  std::thread t1([&] { reorder(start1, end, v1, v2, r1); });
  std::thread t2([&] { reorder(start2, end, v2, v1, r2); });

  for (int i = 0;; i++) {
    // Re-initialize the shared variables
    v1 = 0;
    v2 = 0;

    // Signal the threads to start
    sem_post(&start1);
    sem_post(&start2);

    // Wait for them to finish
    sem_wait(&end);
    sem_wait(&end);

    // Check of both read values bypassed the loads
    auto cond = (r1 == 0) && (r2 == 0);
    if (cond) {
      printf("ERROR! R1 = %d, R2 = %d, ITER %d\n", r1, r2, i);
      assert(false);
    } else
      printf("ALL GOOD! R1 = %d, R2 = %d\n", r1, r2);
  }

  // Should not get here (infinite loop)
  return 0;
}
