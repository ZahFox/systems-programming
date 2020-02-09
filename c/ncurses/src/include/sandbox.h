#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"

typedef struct _tty_size {
  unsigned int cols, rows
} tty_size;

tty_size get_tty_size();

/**
 * Render header displays a title and application stats
 */
void render_header(unsigned int rows, unsigned int cols);

#endif