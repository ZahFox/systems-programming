#include <locale.h>

#include "framebuffer.h"
#include "sandbox.h"
#include "window.h"

void init(void);
void run(void);
int ready_to_exit();

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void)_;
  keep_running = 0;
}

int main(void) {
  signal(SIGINT, sig_handler);

  init();
  run();
  getch();
  endwin();

  return 0;
}

void init(void) {
  setlocale(LC_ALL, "");
  initscr();

  if (has_colors() == FALSE) {
    printf("Your terminal does not support color\n");
    endwin();
    exit(1);
  }

  start_color();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(0);
  noecho();
}

void run(void) {
  WIN win;
  int ch;

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_win_params(&win);
  print_win_params(&win);

  attron(COLOR_PAIR(1));
  printw("Press F2 to exit");
  refresh();
  attroff(COLOR_PAIR(1));

  create_box(&win, TRUE);
  sandbox* s = new_sandbox();

  while ((ch = getch()) != KEY_F(2)) {
    clear();
    tty_size size = get_tty_size();
    render_header(s, size.rows, size.cols);

    wchar_t characters[] = {L'\uE030', L'\u01D5'};
    mvaddch(5, 0, characters[0]);
    mvaddch(5, 5, characters[1]);

    create_box(&win, FALSE);

    switch (ch) {
      case KEY_LEFT:
        --win.startx;
        break;
      case KEY_RIGHT:
        ++win.startx;
        break;
      case KEY_UP:
        --win.starty;
        break;
      case KEY_DOWN:
        ++win.starty;
        break;
    }

    win.startx = CLAMP(win.startx, 0, size.cols - 11);
    win.starty = CLAMP(win.starty, HEADER_ROW_SIZE, size.rows - 4);

    create_box(&win, TRUE);

    msleep(16);
  }

  free(s);
}
