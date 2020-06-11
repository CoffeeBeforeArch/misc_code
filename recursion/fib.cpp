// Simple benchmark for fibonacci
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

int fibonacci(int val) {
  if(val <= 1) return 1;
  return fibonacci(val - 1) + fibonacci(val - 2);
}

static void fibBench(benchmark::State &s) {
  int val = s.range(0);
  int *return_val = new int;
  for(auto _ : s) {
    *return_val = fibonacci(val);
  }
  delete return_val;
}
BENCHMARK(fibBench)->Arg(20);

BENCHMARK_MAIN();
