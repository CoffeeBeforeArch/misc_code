// Short benchmark for strength reduction
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>

#include <vector>

// Benchmark idiv instruction
static void baseMod(benchmark::State &s) {
  std::vector<int> v_in(4096);
  std::vector<int> v_out(4096);

  for (auto _ : s) {
    for (size_t i = 0; i < v_in.size(); i++) v_out[i] = v_in[i] % s.range(0);
  }
}
BENCHMARK(baseMod)->Arg(2)->Unit(benchmark::kMicrosecond);

// Benchmark compiler strength reduction
static void srMod(benchmark::State &s) {
  std::vector<int> v_in(4096);
  std::vector<int> v_out(4096);

  for (auto _ : s) {
    for (size_t i = 0; i < v_in.size(); i++) v_out[i] = v_in[i] % 2;
  }
}
BENCHMARK(altMod)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
