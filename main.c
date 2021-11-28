/*
 * A TUI TODO App.
 * Written by Emir Hurturk on 28.11.2021
 * This is the C version of @tsoder's TUI Todo app which is made in Rust.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/* Error Codes */
#define TOO_MANY_ARGS 1

/* Limitations */
#define MAX_TODO_ITEM 40

#define CTRL(c) ((c) & 037)

struct todo_item {
  char *msg;
  int completed;
};

typedef struct todo_item todo_item_t;

int item_amount = 2;

todo_item_t todo_list[2] = {(todo_item_t) {.msg = "Item 1", .completed=0 }, (todo_item_t) {.msg = "Item 2", .completed = 1}};

void add_todo(todo_item_t item) {
  if (item_amount + 1 > MAX_TODO_ITEM) return;
  todo_list[item_amount + 1] = item;
}

/* Example usage: todoer todo.td */
int main(int argc, char **argv) {

  if (argc != 2) return TOO_MANY_ARGS;

  initscr();
  noecho();
  keypad(stdscr, 1);
  curs_set(0);


  int quit = 0;
  int cur_line = 1;
  addstr("Terminal TODO App: TODOer\n");
  attron(A_UNDERLINE);
  addstr("[ ] Item 1\n");
  attroff(A_UNDERLINE);
  addstr("[x] Item 2\n");
  refresh();
  while(!quit) {

    for (int row = 1; row < item_amount+1; row++) {
      char *it = malloc(sizeof(char)*54);
      sprintf(it, "[%s] %s", !todo_list[row-1].completed ? " " : "x", todo_list[row-1].msg);
      move(row, 0);
      if (cur_line == row)
        attron(A_UNDERLINE);
      addstr(it);
      if (cur_line == row)
        attroff(A_UNDERLINE);
      free(it);
    }

    int ch = getch();
    switch (ch) {
    case 'q':
      quit = 1;
      break;
    case KEY_UP:
      if (cur_line-1 < 1)
        break;
      move(cur_line, 0);
      cur_line--;
      break;
    case KEY_DOWN:
      if (cur_line+1 > item_amount)
        break;
      move(cur_line+1, 0);
      cur_line++;
      break;
    case 10:
      todo_list[cur_line-1].completed = !todo_list[cur_line-1].completed;
      break;
    case CTRL('a'):
      move(++item_amount, 0);
      int x, y;
      addstr("[ ] _________________________");
      refresh();
      getyx(stdscr, y, x);
      /* add item */
      char *input = malloc(sizeof(char)*50);
      int c, l=0;
      while ((c=getch()) != '\n') {
        move(y, l+4);
        addch(c);
        input[l] = c;
        l++;
      }
      clrtoeol();
      add_todo((todo_item_t){.msg=input, .completed=0});
      free(input);
      break;
    }

    refresh();
  }
  endwin();

  return 0;

}
