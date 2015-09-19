#include <kernel/shell.h>

/* get the input.*/
int character[240];

/* the index of 'character'.*/
int index = -1;

/* able to input now, 1 means enable, 0 means disable.*/
int enable = 0;

extern void hello();
extern void hi();

void print() {
  printf("do\n");
}

int wait_for_end() {
  while(enable == 1) {
    if (enable != 1)
      printf("");
  }
  return 0;
}

void input() {
  int c;
  // continue to wait for input, entil we got a '\n'
  while (1) {
    if (c == 5) { // just skip the auto script of gcc.
      printf("  ");
    }
    if (!enable)
      break;
  }

  //  printf("you had input %d characters.\n", index);
  //  printf("character[index - 1]:%c\n", character[index - 1]);
}

/* main process, should never exit.*/
void shell() {
  int had_print_s = 0;
  int hello_str[5] = {'h', 'e', 'l', 'l', 'o'};
  int hi_str[2] = {'h', 'i'};
  int two_str[3] = {'t', 'w', 'o'};

  while(1) {
    // output the 'root@'.
    printf("root@:");

    // get the input.
    index = -1;
    enable = 1;
    input();

    // create new process accoding to the command.
    if (strcmp(hello_str, character, 5)) {

      create_kernel_proc(hello, 100);

    } else if (strcmp(hi_str, character, 2)) {

      create_kernel_proc(hi, 100);

    } else if (strcmp(two_str, character, 3)) {

      create_kernel_proc(hi, 100);
      create_kernel_proc(hello, 100);

    } else if (index >0) {

      printf("unknow command\n");

    } else {

    }

    // wait for the other processess to finished.
    while(proc_num() != 2) {

    }

  }
}
