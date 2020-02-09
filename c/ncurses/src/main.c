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
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(0);
  noecho();
  refresh();
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

    switch (ch) {
      case KEY_LEFT:
        create_box(&win, FALSE);
        --win.startx;
        create_box(&win, TRUE);
        break;
      case KEY_RIGHT:
        create_box(&win, FALSE);
        ++win.startx;
        create_box(&win, TRUE);
        break;
      case KEY_UP:
        create_box(&win, FALSE);
        --win.starty;
        create_box(&win, TRUE);
        break;
      case KEY_DOWN:
        create_box(&win, FALSE);
        ++win.starty;
        create_box(&win, TRUE);
        break;
    }

    msleep(16);
  }

  free(s);
}
