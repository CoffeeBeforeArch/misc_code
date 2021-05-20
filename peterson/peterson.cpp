// Peterson's algorithm
// By: Nick from CoffeeBeforeArch

#include <iostream>
#include <thread>

// Simple class implementing Peterson's algorithm for mutual exclusion
class Peterson {
 private:
  // Is this thread interested in the critical section
  // Needs to be volatile to prevent caching in registers
  volatile int interested[2] = {0, 0};

  // Who's turn is it?
  // Needs to be volatile to prevent caching in registers
  volatile int turn = 0;

 public:
  // Method for locking w/ Peterson's algorithm
  void lock(int tid) {
    // Mark that this thread wants to enter the critical section
    interested[tid] = 1;

    // Assume the other thread has priority
    int other = 1 - tid;
    turn = other;

    // Wait until the other thread finishes or is not interested
    while (turn == other && interested[other])
      ;
  }

  // Method for unlocking w/ Peterson's algorithm
  void unlock(int tid) {
    // Mark that this thread is no longer interested
    interested[tid] = 0;
  }
};

// Work function
void work(Peterson &p, int &val, int tid) {
  for (int i = 0; i < 100000000; i++) {
    // Lock using Peterson's algorithm
    p.lock(tid);
    // Critical section
    val++;
    // Unlock using Peterson's algorithm
    p.unlock(tid);
  }
}

int main() {
  // Shared value
  int val = 0;
  Peterson p;

  // Create threads
  std::thread t0([&] { work(p, val, 0); });
  std::thread t1([&] { work(p, val, 1); });

  // Wait for the threads to finish
  t0.join();
  t1.join();

  // Print the result
  std::cout << "FINAL VALUE IS: " << val << '\n';

  return 0;
}
