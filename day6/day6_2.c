#include <ctype.h>
#include <stdio.h>

typedef long long ll;

int main() {
    ll race_time = 0;
    ll race_distance = 0;

    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char c = '\0';
    short pass = 1;

    while ((c = fgetc(file)) != EOF) {
        if (isdigit(c)) {
            if (pass == 1) {
                race_time = race_time * 10 + (c - '0');
            } else {
                race_distance = race_distance * 10 + (c - '0');
            }
        } else if (c == '\n') {
            pass = 2;
        }
    }

    fclose(file);
    ll n_solutions = 0;

    for (ll charged_ms = 0; charged_ms <= race_time; charged_ms++) {
        ll distance = (race_time - charged_ms) * charged_ms;
        if (distance > race_distance) {
            n_solutions++;
        }
    }

    printf("%lld\n", n_solutions);
    return 0;
}
