#ifndef SANDBOX_H
#define SANDBOX_H

#define MAX_ROW_SIZE 3840
#define MAX_HEADER_SIZE MAX_ROW_SIZE * 3

#include "common.h"

typedef struct __tty_size {
  unsigned int cols, rows;
} tty_size;

typedef struct __sandbox {
  char header[MAX_HEADER_SIZE];
  unsigned int last_col_count;
} sandbox;

sandbox* new_sandbox();

tty_size get_tty_size();

/**
 * Render header displays a title and application stats
 */
void render_header(sandbox* s, unsigned int rows, unsigned int cols);

#endif