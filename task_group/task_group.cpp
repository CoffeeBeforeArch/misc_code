// Simple task group example
// By: Nick from CoffeeBeforeArch

#include <tbb/task_group.h>

#include <cstdio>

// Simple multiply function
void multiply(int i, int j) {
  auto product = i * j;
  printf("Product = %d\n", product);
}

int main() {
  // Create a task group
  tbb::task_group tg;

  // Spawn task for each pair
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      tg.run([=] { multiply(i, j); });
    }
  }

  // Wait for tasks
  tg.wait();

  return 0;
}
