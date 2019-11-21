#include <stdio.h>

void print_bits(size_t const size, void const *const ptr)
{
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  {
    for (j = 7; j >= 0; j--)
    {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }

  puts("");
}

int main(int argv, char *argc[])
{
  int i = 31;
  print_bits(sizeof(i), &i);

  int j = i << 6;
  print_bits(sizeof(j), &j);

  return 0;
  j = i << 16;
  print_bits(sizeof(j), &j);

  j = i << 21;
  print_bits(sizeof(j), &j);
  return 0;
}