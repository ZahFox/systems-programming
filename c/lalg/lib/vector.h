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

IntVec* create_int_vec(int size, int* data);

IntVec* create_zero_int_vec(int size);

int dot_product_int_vec(IntVec* left, IntVec* right);

IntVec* scale_int_vec(IntVec* vec, int scale);

char* str_from_int_vec(IntVec* vec);

void print_int_vec(IntVec* vec);

void flush_vector_buffers();

#endif  // LALG_DEMO_VECTOR_H
