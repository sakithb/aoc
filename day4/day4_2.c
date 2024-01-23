#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_matches(char *line) {
    int matches = 0;
    int c_idx = 0;
    char c;

    short win_list[100] = {0};
    short our_list[100] = {0};
    short num = 0;

    bool is_win = false;
    bool is_our = false;

    while ((c = *(line + c_idx)) != '\0') {
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
                    matches++;
                }

                win_list[i] = 0;
                our_list[i] = 0;
            }

            is_win = false;
            is_our = false;
        }

        c_idx++;
    }

    return matches;
}

int main() {

    FILE *file = fopen("day4_input.txt", "r");

    int total = 0;
    int card_id = 0;

    int matches_size = 1;
    int *matches = calloc(matches_size, sizeof(int));

    int line_size = 200;
    char *line = malloc(line_size);

    memset(matches, 0, sizeof(int));

    while (fgets(line, line_size, file) != NULL) {

        if (strlen(line) >= line_size) {
            line_size = strlen(line) + 1;
            char *tmp = realloc(line, line_size);

            if (tmp != NULL) {
                line = tmp;
            } else {
                free(line);
                free(matches);
                fclose(file);

                printf("Error reallocating memory!\n");
                return 1;
            }
        }

        int card_matches = get_matches(line);
        int diff = (card_id + card_matches + 1) - matches_size;

        if (diff > 0) {
            matches_size += diff;
            int *tmp = realloc(matches, matches_size * sizeof(int));

            if (tmp != NULL) {
                matches = tmp;
                memset(matches + (matches_size - diff), 0, diff * sizeof(int));
            } else {
                free(line);
                free(matches);
                fclose(file);

                printf("Error reallocating memory!\n");
                return 1;
            }
        }

        matches[card_id] += 1;

        for (int i = matches[card_id]; i > 0; i--) {
            int tmp = card_matches;
            while (tmp > 0) {
                matches[card_id + tmp] += 1;
                tmp--;
            }
        }

        card_id++;
    }

    while (matches_size > 0) {
        total += matches[matches_size - 1];
        matches_size--;
    }

    free(matches);
    free(line);
    fclose(file);

    printf("Total matches: %d\n", total);

    return 0;
}
