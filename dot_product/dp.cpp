// This program implements two dot product implementations in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <execution>
#include <vector>

// Classic C-style dot product
float dot_product1(std::vector<float> &__restrict v1,
                   std::vector<float> &__restrict v2) {
  float tmp = 0.0;
  for (size_t i = 0; i < v1.size(); i++) {
    tmp += v1[i] * v2[i];
  }
  return tmp;
}

// Modern C++ dot product
float dot_product2(std::vector<float> &__restrict v1,
                   std::vector<float> &__restrict v2) {
  return std::transform_reduce(std::execution::unseq, begin(v1), end(v1),
                               begin(v2), 0.0f);
}

// Modern C++ dot product
float dot_product4(std::vector<float> &__restrict v1,
                   std::vector<float> &__restrict v2) {
  return std::transform_reduce(std::execution::unseq, begin(v1), end(v1),
                               begin(v2), 0.0);
}


// Hand-vectorized dot product
float dot_product3(const float *__restrict v1, const float *v2,
                   const size_t N) {
  auto tmp = 0;
  for (size_t i = 0; i < N; i += 8) {
    // Temporary variables to help with intrinsic
    float r[8];
    __m256 rv;

    // Our dot product intrinsic
    rv = _mm256_dp_ps(_mm256_load_ps(v1 + i), _mm256_load_ps(v2 + i), 0xf1);

    // Avoid type punning using memcpy
    std::memcpy(r, &rv, sizeof(float) * 8);

    tmp += r[0] * r[4];
  }
  return tmp;
}

// Benchmark the baseline C-style dot product
static void baseDP(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  std::vector<float> v1;
  std::fill_n(std::back_inserter(v1), N, rand() % 100);
  std::vector<float> v2;
  std::fill_n(std::back_inserter(v2), N, rand() % 100);

  // Keep the result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product1(v1, v2);
  }
}
BENCHMARK(baseDP)->DenseRange(8, 10);

// Benchmark the modern C++ dot product
static void modernDP(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  std::vector<float> v1;
  std::fill_n(std::back_inserter(v1), N, rand() % 100);
  std::vector<float> v2;
  std::fill_n(std::back_inserter(v2), N, rand() % 100);

  // Keep our result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product2(v1, v2);
  }
}
BENCHMARK(modernDP)->DenseRange(8, 10);

// Benchmark the modern C++ dot product
static void modernDP_double(benchmark::State &s) {
  // Get the size of the vector
  size_t N = 1 << s.range(0);

  // Initialize the vectors
  std::vector<float> v1;
  std::fill_n(std::back_inserter(v1), N, rand() % 100);
  std::vector<float> v2;
  std::fill_n(std::back_inserter(v2), N, rand() % 100);

  // Keep our result from being optimized away
  volatile float result = 0;

  // Our benchmark loop
  while (s.KeepRunning()) {
    result = dot_product4(v1, v2);
  }
}
BENCHMARK(modernDP_double)->DenseRange(8, 10);

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
    result = dot_product3(v1, v2, N);
  }
}
BENCHMARK(handTunedDP)->DenseRange(8, 10);

// Our benchmark main function
BENCHMARK_MAIN();
