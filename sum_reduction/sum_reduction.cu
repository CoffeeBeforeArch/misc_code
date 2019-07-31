// This program implements a simple, but flexible sum reduction
// implementation in CUDA
// By: Nick from CoffeeBeforeArch

#include <iostream>
#include <cstdlib>
#include <cassert>

// Each TB needs some shared memory
// Allocate for 256 ints
#define SHMEM_SIZE 256 * 4

using namespace std;

// Sum reduction kernel taken from previous CUDA video
// Slightly modified to handle inputs of not powers of 256
__global__ void sum_reduction(int *v_in, int *v_out, int N){
	// Allocate shared memory statically
	__shared__ int partial_sum[SHMEM_SIZE];

	// Calculate thread ID
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
   
    // Mask off inactive threads in a TB
    if(tid < N){
	    // Load elements into shared memory
	    partial_sum[threadIdx.x] = v_in[tid];
	    __syncthreads();

        // How many elements we sum in this TB depends on how many remaining
        // elements there are. If 256, use blockDim, else, use N
        int max = (N < blockDim.x) ? N : blockDim.x;

	    // Iterate of log base 2 the block dimension
	    for (int s = 1; s < max; s *= 2) {
		    // Reduce the threads performing work by half previous the previous
		    // iteration each cycle
		    if (threadIdx.x % (2 * s) == 0) {
			    partial_sum[threadIdx.x] += partial_sum[threadIdx.x + s];
		    }
		    __syncthreads();
	    }

	    // Let the thread 0 for this block write it's result to main memory
	    // Result is inexed by this block
	    if (threadIdx.x == 0) {
		    v_out[blockIdx.x] = partial_sum[0];
	    }
    }
}

int main(){
    // Number of elements
    int N = 1 << 20;
    int CPU_SUM = N;
    size_t bytes = N * sizeof(int);

    // Host arrays
    int *h_v_in = new int[N];
    int *h_v_out = new int[1];

    // Device arrays
    int *d_v_in, *d_v_out;
    cudaMalloc(&d_v_in, bytes);
    cudaMalloc(&d_v_out, bytes);

    // Init input array
    for(int i = 0; i < N; i++){
        h_v_in[i] = 1;
    }

    // Copy the array over
    cudaMemcpy(d_v_in, h_v_in, bytes, cudaMemcpyHostToDevice);

    // TB size;
    int THREADS = 256;

    // Track the number of iterations it takes
    int iter = 0;
    
    // Grid size may change each loop iteration
    int GRID;

    // Simple loop to keep launching kernels until we're done
    // N == 1 means we only have 1 elements left, aka, we're done
    // If it's not a power of 256, N will converge to 0 instead
    while(N > 1){
        // Calculate the grid size
        GRID = (N + THREADS - 1) / THREADS;
        
        // Alternate kernel inputs
        // This is so we don't need to constantly re-allocate new
        // output arrays
        if(iter % 2){
            sum_reduction<<<GRID, THREADS>>>(d_v_out, d_v_in, N);
        }else{
            sum_reduction<<<GRID, THREADS>>>(d_v_in, d_v_out, N);
        }

        // LOG_256(N) iterations of the loop
        iter++;
        N /= 256;
    }

    // Which array we copy back from depends on the final iter #
    // Only a single sum needs to be copied out
    if(iter % 2){
        cudaMemcpy(h_v_out, d_v_out, sizeof(int), cudaMemcpyDeviceToHost);
    }else{
        cudaMemcpy(h_v_out, d_v_in, sizeof(int), cudaMemcpyDeviceToHost);
    }

    // Print the result
    cout << "Number of iterations = " << iter << endl;
    cout << "Reduced Sum = " << h_v_out[0] << endl;
    cout << "CPU sum = " << CPU_SUM << endl;

    return 0;
}
