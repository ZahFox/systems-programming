#include "vector.h"

int main() {
  printf("Hello, World!\n");
  int data[4] = {4, 5, 7, 8};
  IntVec* vec = create_int_vec(4, data);
  print_int_vec(vec);
  return 0;
}