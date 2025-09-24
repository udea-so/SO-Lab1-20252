#include <stdio.h>

void show(int x);
int add(int x, int y);

int main() {
  int f = 10;
  int g = 20;
  int h = add(f, g);
  show(h); // to print some output to screen
  int i = add(2, 3);
  show(i);
  return 0;
}

int add(int x, int y) {
  return x + y;
}

void show(int x) {
  printf("value: %d\n", x);
}

