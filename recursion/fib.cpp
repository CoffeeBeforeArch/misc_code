// Simple benchmark for fibonacci
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

// Recursive fibonacci function
int fibonacci(int val) {
  if(val <= 1) return 1;
  return fibonacci(val - 1) + fibonacci(val - 2);
}

// Fibonacci Benchmark
static void fibBench(benchmark::State &s) {
  // Get our input value
  int val = s.range(0);

  // Use dynmaically allocated memory to keep it from being optimized out
  int *return_val = new int;

  // Main benchmark loop
  for(auto _ : s) {
    *return_val = fibonacci(val);
  }

  // Free memory
  delete return_val;
}
BENCHMARK(fibBench)->Arg(20);

// Benchmark main function
BENCHMARK_MAIN();
