// This program implements two dot product implementations in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <immintrin.h>

#include <cstdlib>
#include <cstring>
#include <vector>

// Hand-vectorized dot product
float dot_product(const float *__restrict v1, const float *v2, const size_t N) {
  auto tmp = 0.0f;
  for (size_t i = 0; i < N; i += 8) {
    // Temporary variables to help with intrinsic
    float r[8];
    __m256 rv;

    // Our dot product intrinsic
    rv = _mm256_dp_ps(_mm256_load_ps(v1 + i), _mm256_load_ps(v2 + i), 0xf1);

    // Avoid type punning using memcpy
    std::memcpy(r, &rv, sizeof(float) * 8);

    tmp += r[0] + r[4];
  }
  return tmp;
}

// Benchmark our hand-tuned dot product
static void handTunedDP(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  // Align memory to 32 bytes for the vector instruction
  float *v1 = (float *)aligned_alloc(32, N * sizeof(float));
  float *v2 = (float *)aligned_alloc(32, N * sizeof(float));
  for (size_t i = 0; i < N; i++) {
    v1[i] = rand() % 100;
    v2[i] = rand() % 100;
  }

  // Keep our result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product(v1, v2, N);
  }
}
BENCHMARK(handTunedDP)->DenseRange(8, 10);

// Our benchmark main function
BENCHMARK_MAIN();
