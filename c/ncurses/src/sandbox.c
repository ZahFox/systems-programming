#include "sandbox.h"

tty_size get_tty_size() {
  char* tty;
  int fd = 0;
  int result = 0;
  struct winsize win;

  tty_size size = {.cols = 0, .rows = 0};

  tty = ttyname(0);
  if (!tty) {
    return size;
  }

  fd = open(tty, O_RDWR);
  if (fd == -1) {
    return size;
  }

  result = ioctl(fd, TIOCGWINSZ, &win);
  if (result == -1) {
    return size;
  }

  size.cols = win.ws_col;
  size.rows = win.ws_row;
  return size;
}

sandbox* new_sandbox() {
  sandbox* instance = malloc(sizeof(sandbox));
  instance->last_col_count = 0;
  return instance;
}

void render_header(sandbox* s, unsigned int rows, unsigned int cols) {
  if (rows < 3) {
    return;
  }

  if (s->last_col_count == cols) {
    mvprintw(0, 0, "%s", s->header);
    return;
  }

  s->last_col_count = cols;
  int padding = 2;
  int content_width = cols - (padding * 2);
  char content[100];
  sprintf(content, " ( NCURSES SANDBOX )  rows: %u    cols: %u ", rows, cols);

  // Draw the first row
  for (int i = 0; i < cols; i++) {
    s->header[i] = '=';
  }

  // Draw the second row
  int content_length = strlen(content);
  for (int i = cols, j = 0, c = 0; i < cols * 2; i++, j++) {
    if (j < padding || j + padding > cols - 1) {
      s->header[i] = '=';
    } else {
      if (c < content_length - 1) {
        s->header[i] = content[c++];
      } else {
        s->header[i] = ' ';
      }
    }
  }

  // Draw the third row
  for (int i = (cols * 2); i < (cols * 3); i++) {
    s->header[i] = '=';
  }

  s->header[(cols * 3)] = '\0';
  mvprintw(0, 0, "%s", s->header);
}