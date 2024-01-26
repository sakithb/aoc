#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;

typedef struct {
    int time;
    int distance;
} Race;

typedef struct {
    int size;
    Race *races;
} Races;

void add_race(int time, int distance, Races *races) {
    Race race;
    race.time = time;
    race.distance = distance;

    races->size += 1;
    Race *tmp_races = realloc(races->races, races->size * sizeof(Race));

    if (tmp_races == NULL) {
        longjmp(exit_buf, 1);
    } else {
        races->races = tmp_races;
        races->races[races->size - 1] = race;
    }
}

int get_race_n_solutions(Race *race) {
    int n_solutions = 0;

    for (int charged_ms = 0; charged_ms <= race->time; charged_ms++) {
        int distance = (race->time - charged_ms) * charged_ms;
        if (distance > race->distance) {
            n_solutions++;
        }
    }

    return n_solutions;
}

int main() {
    Races races;
    races.size = 0;
    races.races = calloc(0, sizeof(Race));

    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = calloc(file_size + 1, sizeof(char));
    fread(input, sizeof(char), file_size, file);
    fclose(file);

    if (setjmp(exit_buf) != 0) {
        free(input);
        free(races.races);
        return 0;
    }

    char *c = input;
    char *s = NULL;

    int i = 0;
    int pass = 1;

    while (*c != '\0') {
        if (isdigit(*c)) {
            char *nc = c + 1;
            if (s == NULL) {
                s = c;
            }

            if (*nc == ' ' || *nc == '\n' || *nc == '\0') {
                char str[c - s + 2];
                strncpy(str, s, c - s + 1);
                str[c - s + 1] = '\0';

                int num = atoi(str);

                if (i < races.size) {
                    if (pass == 1) {
                        races.races[i].time = num;
                    } else {
                        races.races[i].distance = num;
                    }
                } else {
                    if (pass == 1) {
                        add_race(num, 0, &races);
                    } else {
                        add_race(0, num, &races);
                    }
                }

                s = NULL;
                i++;
            }
        } else {
            if (*c == '\n') {
                i = 0;
                pass = 2;
            }

            s = NULL;
        }

        c++;
    }

    int ans = 1;

    for (int i = 0; i < races.size; i++) {
        ans *= get_race_n_solutions(&races.races[i]);
    }

    printf("%d\n", ans);

    longjmp(exit_buf, 1);
}
