#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

typedef struct _win_border_struct {
  chtype ls, rs, ts, bs, tl, tr, bl, br;
} WIN_BORDER;

typedef struct _WIN_struct {
  int startx, starty;
  int height, width;
  WIN_BORDER border;
} WIN;

void init_win_params(WIN* p_win);
void print_win_params(WIN* p_win);
void create_box(WIN* win, bool flag);

#endif