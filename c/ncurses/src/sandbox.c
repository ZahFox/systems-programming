#include "sandbox.h"

#define MAX_ROW_SIZE 3840
#define MAX_HEADER_SIZE MAX_ROW_SIZE * 3

tty_size get_tty_size() {
  char* tty;
  int fd = 0;
  int result = 0;
  struct winsize win;

  tty = ttyname(0);
  if (!tty) {
    return;
  }

  fd = open(tty, O_RDWR);
  if (fd == -1) {
    return;
  }

  result = ioctl(fd, TIOCGWINSZ, &win);
  if (result == -1) {
    return;
  }

  tty_size size = {.cols = win.ws_col, .rows = win.ws_row};
  return size;
}

static char header[MAX_HEADER_SIZE];
static unsigned int last_col_count = 0;

void render_header(unsigned int rows, unsigned int cols) {
  if (rows < 3) {
    return;
  }

  if (last_col_count == cols) {
    mvprintw(0, 0, "%s", header);
    return;
  }

  last_col_count = cols;
  int padding = 2;
  int content_width = cols - (padding * 2);
  char content[100];
  sprintf(content, " ( NCURSES SANDBOX )  rows: %u    cols: %u ", rows, cols);

  // Draw the first row
  for (int i = 0; i < cols; i++) {
    header[i] = '=';
  }

  // Draw the second row
  int content_length = strlen(content);
  for (int i = cols, j = 0, c = 0; i < cols * 2; i++, j++) {
    if (j < padding || j + padding > cols - 1) {
      header[i] = '=';
    } else {
      if (c < content_length - 1) {
        header[i] = content[c++];
      } else {
        header[i] = ' ';
      }
    }
  }

  // Draw the third row
  for (int i = (cols * 2); i < (cols * 3); i++) {
    header[i] = '=';
  }

  header[(cols * 3)] = '\0';
  mvprintw(0, 0, "%s", header);
}