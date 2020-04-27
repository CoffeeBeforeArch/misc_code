// This program performs sum reduction with an optimization
// removing warp bank conflicts
// By: Nick from CoffeeBeforeArch

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

#define SIZE 256

__global__ void sum_reduction(int *v, int *v_r, int n) {
  // Calculate thread ID
  int tid = blockIdx.x * blockDim.x + threadIdx.x;

  // Boundary check
  if (tid < n) {
    // Allocate shared memory
    __shared__ int partial_sum[SIZE];

    // Calculate the number of elements this block reduces
    // Only the last block may have stragglers
    int reduce_elements;
    if (blockIdx.x == gridDim.x - 1) {
      reduce_elements = n - blockIdx.x * SIZE;
    } else {
      reduce_elements = SIZE;
    }

    // Find the next power of two
    // __clz finds the leading number of zeros in an int
    int next_power = 1 << (32 - __clz(reduce_elements) + 1);
    int init = next_power > SIZE ? SIZE : next_power;

    // Load elements into shared memory
    partial_sum[threadIdx.x] = v[tid];
    __syncthreads();

    // Start with a padded number of reduce_elements
    for (int s = init; s > 0; s >>= 1) {
      // Only threads < stride compute partial sums
      // Only threads accessing elements < reduce_elements need to be active
      // This handles the case where reduce_elements is an odd number
      if (threadIdx.x < s && threadIdx.x + s < reduce_elements) {
        partial_sum[threadIdx.x] += partial_sum[threadIdx.x + s];
      }
      __syncthreads();
    }

    // Let the thread 0 for this block write it's result to main memory
    // Result is inexed by this block
    if (threadIdx.x == 0) {
      v_r[blockIdx.x] = partial_sum[0];
    }
  }
}

int main() {
  // Vector size
  int n = 1 << 20;
  size_t bytes = n * sizeof(int);

  // Host-side input vector
  std::vector<int> h_v(n);
  std::generate(begin(h_v), end(h_v), []() { return 1; });

  // Single result element
  int h_v_r = 0;

  // Allocate device memory
  int *d_v, *d_v_r;
  cudaMalloc(&d_v, bytes);
  cudaMalloc(&d_v_r, bytes);

  // Copy to device
  cudaMemcpy(d_v, h_v.data(), bytes, cudaMemcpyHostToDevice);

  // TB Size
  int TB_SIZE = SIZE;

  // Grid Size
  int GRID_SIZE = (n + TB_SIZE - 1) / TB_SIZE;

  // Number of elements reduced in the next iteration
  int num_elements = n;

  // Launch kernels until we've performed the complete reduction
  while (1) {
    // Call kernel
    sum_reduction<<<GRID_SIZE, TB_SIZE>>>(d_v, d_v_r, num_elements);

    // No more reductions left!
    if (GRID_SIZE == 1) break;

    // Swap the pointers each iteration
    // Output from last iteration is the input to the next
    std::swap(d_v, d_v_r);

    // Calculate the number of elements next iteration
    // Number of input elements next iteration is the number of output
    // elements from last iteration
    num_elements = GRID_SIZE;

    // Calculate padded grid size
    GRID_SIZE = (GRID_SIZE + TB_SIZE - 1) / TB_SIZE;
  }

  // Copy the result back to the host
  cudaMemcpy(&h_v_r, d_v_r, sizeof(int), cudaMemcpyDeviceToHost);

  // Host sum
  int res = std::accumulate(begin(h_v), end(h_v), 0);

  // Check the result
  assert(h_v_r == res);

  std::cout << "COMPLETED SUCCESSFULLY!\n";

  return 0;
}
