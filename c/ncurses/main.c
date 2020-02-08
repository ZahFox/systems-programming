#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void init(void);
void run(void);
void run2(void);
void run3(void);
int mult(int, int);

int main(void) {
  init();
  run3();
  getch();
  endwin();

  return 0;
}

void init(void) {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("WELCOME STRANGER\n");
  refresh();
}

void run2(void) {
  char msg[] = "Enter a string: ";
  char str[80];
  int row, col;

  getmaxyx(stdscr, row, col);
  mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);
  getstr(str);
  mvprintw(LINES - 2, 0, "You Entered: %s", str);
}

void run3(void) {
  int row, col;
  for (int i = 0; i < 30; i++) {
    getmaxyx(stdscr, row, col);
    mvprintw(0, 0, "ROWS: %d, COLS: %d", row, col);

    if (!strcmp(keyname(getch()), "^C")) {
      return;
    }

    refresh();
    sleep(1);
  }
}

void run(void) {
  char msg[20];

  for (int i = 0; i < 20; i++) {
    int val = mult(i + 1, i + 1);
    sprintf(msg, "message number %d\n", val);
    printw(msg);
    refresh();
    sleep(1);
  }
}

int mult(int l, int r) { return l * r; }
