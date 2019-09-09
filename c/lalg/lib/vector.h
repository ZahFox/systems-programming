//
// Created by zahfox on 9/8/19.
//

#ifndef LALG_DEMO_VECTOR_H
#define LALG_DEMO_VECTOR_H

#include "includes.h"

typedef struct IntVec {
  const int rows;
  int* data;
} IntVec;

IntVec* create_int_vec(int size, int* data) {
  IntVec* vec = malloc(sizeof(IntVec));
  vec->data = data;
  *(int*)&vec->rows = size;
  return vec;
}

void print_int_vec(void* vec_ptr) {
  IntVec* vec = (IntVec*)vec_ptr;
  int row_count = vec->rows;
  int char_count = 3 + (row_count * 2) + ((row_count - 1) * 2);
  char msg[char_count + 1];
  msg[0] = '[';
  int next_token = 1;

  for (int i = 0; i < row_count - 1; i++) {
    msg[next_token] = '%';
    msg[next_token + 1] = 'd';
    msg[next_token + 2] = ',';
    msg[next_token + 3] = ' ';
    next_token = next_token + 4;
  }

  msg[next_token] = '%';
  msg[next_token + 1] = 'd';
  msg[next_token + 2] = ']';
  msg[next_token + 3] = ';';
  msg[next_token + 4] = '\0';

  char output[char_count + 1 + (row_count * 10)];
  int* data = vec->data;
  switch (row_count) {
    case 1: {
      sprintf(output, msg, *data);
      break;
    }
    case 2: {
      sprintf(output, msg, *data, *(data + 1));
      break;
    }
    case 3: {
      sprintf(output, msg, *data, *(data + 1), *(data + 2));
      break;
    }
    case 4: {
      sprintf(output, msg, *data, *(data + 1), *(data + 2), *(data + 3));
      break;
    }
  }

  puts(output);
}

#endif  // LALG_DEMO_VECTOR_H
