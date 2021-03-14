// This example shows off a memory problem on x86
// By: Nick from CoffeeBeforeArch

#include <emmintrin.h>

#include <cassert>
#include <cstdio>
#include <semaphore>
#include <thread>

void reorder(std::binary_semaphore &start, std::counting_semaphore<2> &end,
             int &v1, int &v2, int &rec) {
  // Keep going forever
  while (true) {
    // Wait for the signal to start
    start.acquire();

    // Write to v2
    v1 = 1;

    // Barrier to prevent re-ordering in the hardware!
    _mm_mfence();

    // Read v1
    rec = v2;

    // Say we're done for this iteration
    end.release();
  }
}

int main() {
  // Semaphores for signaling threads
  std::binary_semaphore s1(0);
  std::binary_semaphore s2(0);
  std::counting_semaphore<2> e(0);

  // Variable for memory re-ordering
  // Use alignas to put on different cache lines
  alignas(64) int v1 = 0, v2 = 0;
  alignas(64) int r1 = 0, r2 = 0;

  // Start threads
  std::thread t1([&] { reorder(s1, e, v1, v2, r1); });
  std::thread t2([&] { reorder(s2, e, v2, v1, r2); });

  for (int i = 0;; i++) {
    // Re-initialize the shared variables
    v1 = 0;
    v2 = 0;

    // Signal the threads to start
    s1.release();
    s2.release();

    // Wait for them to finish
    e.acquire();
    e.acquire();

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
