// Baseline cache-tiled GEMM using a simple work queue
// By: Nick from CoffeeBeforeArch

#include <algorithm>
#include <atomic>
#include <random>
#include <thread>
#include <vector>

// Blocked column parallel implementation w/ atomics
void blocked_column_multi_output_parallel_atomic_gemm(
    const double *A, const double *B, double *C, std::size_t N,
    std::atomic<uint64_t> &pos) {
  // For each chunk of columns
  for (std::size_t col_chunk = pos.fetch_add(16); col_chunk < N;
       col_chunk = pos.fetch_add(16))
    // For each chunk of rows
    for (std::size_t row_chunk = 0; row_chunk < N; row_chunk += 16)
      // For each block of elements in this row of this column chunk
      // Solve for 16 elements at a time
      for (std::size_t tile = 0; tile < N; tile += 16)
        // For apply that tile to each row of the row chunk
        for (std::size_t row = 0; row < 16; row++)
          // For each row in the tile
          for (std::size_t tile_row = 0; tile_row < 16; tile_row++)
            // Solve for each element in this tile row
            for (std::size_t idx = 0; idx < 16; idx++)
              C[(row + row_chunk) * N + col_chunk + idx] +=
                  A[(row + row_chunk) * N + tile + tile_row] *
                  B[tile * N + tile_row * N + col_chunk + idx];
}

int main() {
  // Size of our matrix
  std::size_t N = (1 << 12) + 16;

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<double> dist(-10, 10);

  // Create input matrices
  double *A = static_cast<double *>(aligned_alloc(64, N * N * sizeof(double)));
  double *B = static_cast<double *>(aligned_alloc(64, N * N * sizeof(double)));
  double *C = static_cast<double *>(aligned_alloc(64, N * N * sizeof(double)));

  // Initialize them with random values (and C to 0)
  std::generate(A, A + N * N, [&] { return dist(rng); });
  std::generate(B, B + N * N, [&] { return dist(rng); });
  std::generate(C, C + N * N, [&] { return 0; });

  // Set up for launching threads
  std::size_t num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  // Atomic uint64_t to keep track of position
  std::atomic<uint64_t> pos{0};

  // Launch threads
  for (std::size_t i = 0; i < num_threads - 1; i++) {
    threads.emplace_back([&] {
      blocked_column_multi_output_parallel_atomic_gemm(A, B, C, N, pos);
    });
  }
  blocked_column_multi_output_parallel_atomic_gemm(A, B, C, N, pos);

  // Wait for all threads to complete
  for (auto &t : threads) t.join();

  // Clear the threads each iteration of the benchmark
  threads.clear();

  // Free memory
  free(A);
  free(B);
  free(C);

  return 0;
}
