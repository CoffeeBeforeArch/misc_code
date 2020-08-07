// A bad way for threads to write to the same memory location
// By: Nick from CoffeeBeforeArch

#include <iostream>
#include <thread>

int main() {
  // Shared value for our threads
  int shared_val = 0;

  // Number of iterations (65536)
  int N = 1 << 20;

  // Lambda that performs an increment
  auto inc_func = [&]() {
    for (auto i = 0; i < N; i++) shared_val++;
  };

  // Create two threads
  std::thread t1(inc_func);
  std::thread t2(inc_func);

  // Join the threads
  t1.join();
  t2.join();

  // Print the result
  std::cout << "FINAL VALUE IS: " << shared_val << '\n';

  return 0;
}
