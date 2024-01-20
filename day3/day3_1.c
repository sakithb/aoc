#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char c_at_pos(char *contents, int x, int y, int width, int height) {
  if (x < 0 || y < 0 || x > width || y > height) {
    return '\0';
  }

  int pos = (y * width) + x;
  return contents[pos];
}

bool is_symbol(char c) { return ispunct(c) > 0 && c != '.'; }
bool is_part_number(char *contents, int x, int y, int width, int height) {

  char tl = c_at_pos(contents, x - 1, y - 1, width, height);
  char tt = c_at_pos(contents, x, y - 1, width, height);
  char tr = c_at_pos(contents, x + 1, y - 1, width, height);
  char ll = c_at_pos(contents, x - 1, y, width, height);
  char rr = c_at_pos(contents, x + 1, y, width, height);
  char bl = c_at_pos(contents, x - 1, y + 1, width, height);
  char bb = c_at_pos(contents, x, y + 1, width, height);
  char br = c_at_pos(contents, x + 1, y + 1, width, height);

  return is_symbol(tl) || is_symbol(tt) || is_symbol(tr) || is_symbol(ll) ||
         is_symbol(rr) || is_symbol(bl) || is_symbol(bb) || is_symbol(br);
}

int main() {
  int sum = 0;

  FILE *file = fopen("./day3_input.txt", "r");
  fseek(file, 0, SEEK_END);

  int size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *contents = malloc(size + 1);
  char *line = malloc(size + 1);

  int width = 0;
  int height = 0;

  while ((line = fgets(line, size, file)) != NULL) {
    height++;
    width = strcspn(line, "\r\n");
    line[width] = '\0';

    strcat(contents, line);
  }

  int num_start = -1;
  bool num_valid = false;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width + 1; x++) {
      char c = c_at_pos(contents, x, y, width, height);

      if (isdigit(c) && x < width) {
        if (num_start == -1) {
          num_start = y * width + x;
        }

        if (is_part_number(contents, x, y, width, height)) {
          num_valid = true;
        }
      } else {
        if (num_start > -1 && num_valid) {
          int num_end = y * width + x;
          int num_len = num_end - num_start;

          char num_str[num_len];
          strncpy(num_str, contents + num_start, num_len);
          num_str[num_len] = '\0';

          sum += atoi(num_str);
        }

        num_valid = false;
        num_start = -1;
      }
    }

    num_valid = false;
    num_start = -1;
  }

  free(line);
  free(contents);

  printf("Part 1: %d\n", sum);

  return 0;
}
