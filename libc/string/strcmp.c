#include <string.h>

/* if they are equal, return 1.
 * or return 0. */
int strcmp(int* stra, int* strb, int size_s) {
  int n = 0;
  while(n < size_s &&stra[n] == strb[n]) {
    //    printf("%c == %c\n", stra[n],strb[n]);
    n++;
  }
  return n == size_s;
}
