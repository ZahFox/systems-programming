#include "vector.h"

int main() {
  int data[4] = {4, 5, 7, 8};
  IntVec* vec1 = create_int_vec(4, data);
  IntVec* vec2 = create_int_vec(4, data);

  print_int_vec(vec1);
  char* vec2_str = str_from_int_vec(vec2);
  puts(vec2_str);

  printf("vec1 * vec2 = %d\n", dot_product_int_vec(vec1, vec2));
  printf("%s * 2 = %s\n", vec2_str, str_from_int_vec(scale_int_vec(vec2, 2)));
  printf("vec1 * vec2 = %d\n", dot_product_int_vec(vec1, vec2));

  free(vec2_str);
  flush_vector_buffers();
  return 0;
}