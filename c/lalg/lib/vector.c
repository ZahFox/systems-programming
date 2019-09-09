//
// Created by zahfox on 9/8/19.
//

#include "vector.h"
#include "error.h"

static IntVec* INT_VECTOR_BUFFER[1000];
u_int8_t INT_VECTOR_BUFFER_INDEX = 0;

void flush_vector_buffers() {
  for (int i = 0; i < INT_VECTOR_BUFFER_INDEX; i++) {
    IntVec* vec = INT_VECTOR_BUFFER[i];
    free(vec->data);
    free(vec);
  }
  INT_VECTOR_BUFFER_INDEX = 0;
}

IntVec* create_int_vec(int size, int* data) {
  if (size < 1 || size > 4) {
    throw_err(LALG_ERR_VEC_SIZE);
  }

  IntVec* vec = malloc(sizeof(IntVec));
  int data_size = sizeof(int) * size;
  vec->data = malloc(data_size);
  memcpy(vec->data, data, data_size);
  *(int*)&vec->rows = size;

  INT_VECTOR_BUFFER[INT_VECTOR_BUFFER_INDEX] = vec;
  INT_VECTOR_BUFFER_INDEX++;
  return vec;
}

IntVec* create_zero_int_vec(int size) {
  int data[size];
  for (int i = 0; i < size; i++) {
    data[i] = 0;
  }
  return create_int_vec(size, data);
}

int dot_product_int_vec(IntVec* left, IntVec* right) {
  int result = 0;
  int row_count = left->rows <= right->rows ? left->rows : right->rows;
  for (int i = 0; i < row_count; i++) {
    result += (*((left->data) + i)) * (*((right->data) + i));
  }
  return result;
}

IntVec* scale_int_vec(IntVec* vec, int scale) {
  int* data = vec->data;
  for (int i = 0; i < vec->rows; i++) {
    *(data + i) = *(data + i) * scale;
  }
  return vec;
}
char* str_from_int_vec(IntVec* vec) {
  int row_count = vec->rows;
  int char_count = 2 + (row_count * 2) + ((row_count - 1) * 2);
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
  msg[next_token + 4] = '\0';

  char* output = malloc(char_count + 1 + (row_count * 10));
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
  return output;
}

void print_int_vec(IntVec* vec) {
  char* str = str_from_int_vec(vec);
  puts(str);
  free(str);
}