#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    int sum = 0;

    FILE *file = fopen("day4_input.txt", "r");

    char c;
    short win_list[100] = {0};
    short our_list[100] = {0};
    short num = 0;

    int pts = 0;
    bool is_win = false;
    bool is_our = false;

    while ((c = fgetc(file)) != EOF) {
        if (num > 0 && (c == ' ' || c == '\n')) {
            if (is_win) {
                win_list[num]++;
            } else if (is_our) {
                our_list[num]++;
            }

            num = 0;
        }

        if ((is_win || is_our) && isdigit(c)) {
            num = num * 10 + (c - '0');
        } else if (c == '|') {
            is_win = false;
            is_our = true;
        } else if (c == ':') {
            is_win = true;
            is_our = false;
        } else if (c == '\n') {
            for (int i = 0; i < 100; i++) {
                if (our_list[i] > 0 && win_list[i] > 0) {
                    pts = (pts >= 1) ? pts * 2 : 1;
                }

                win_list[i] = 0;
                our_list[i] = 0;
            }

            sum += pts;
            pts = 0;

            is_win = false;
            is_our = false;
        }
    }

    fclose(file);
    printf("Sum: %d\n", sum);

    return 0;
}
