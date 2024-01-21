#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point {
    int x;
    int y;
};

struct Dimensions {
    int width;
    int height;
};

char c_at_pos(char *contents, struct Point p, struct Dimensions d) {
    if (p.x < 0 || p.y < 0 || p.x >= d.width || p.y >= d.height) {
        return '\0';
    }

    return contents[p.y * d.width + p.x];
}

struct Point get_asterik(char *contents, struct Point p, struct Dimensions d) {
    struct Point tl = {p.x - 1, p.y - 1};
    struct Point tt = {p.x, p.y - 1};
    struct Point tr = {p.x + 1, p.y - 1};
    struct Point ll = {p.x - 1, p.y};
    struct Point rr = {p.x + 1, p.y};
    struct Point bl = {p.x - 1, p.y + 1};
    struct Point bb = {p.x, p.y + 1};
    struct Point br = {p.x + 1, p.y + 1};

    if (c_at_pos(contents, tl, d) == '*') {
        return tl;
    } else if (c_at_pos(contents, tt, d) == '*') {
        return tt;
    } else if (c_at_pos(contents, tr, d) == '*') {
        return tr;
    } else if (c_at_pos(contents, ll, d) == '*') {
        return ll;
    } else if (c_at_pos(contents, rr, d) == '*') {
        return rr;
    } else if (c_at_pos(contents, bl, d) == '*') {
        return bl;
    } else if (c_at_pos(contents, bb, d) == '*') {
        return bb;
    } else if (c_at_pos(contents, br, d) == '*') {
        return br;
    } else {
        return (struct Point){-1, -1};
    }
}

int get_number(char *contents, struct Point p, struct Dimensions d) {
    char c = c_at_pos(contents, p, d);

    char *l = contents + (p.y * d.width) + p.x;
    char *r = contents + (p.y * d.width) + p.x;

    for (int lc = p.x; lc > 0 && isdigit(*(l - 1)); lc--) {
        l--;
    }

    for (int rc = d.width - p.x - 1; rc > 0 && isdigit(*(r + 1)); rc--) {
        r++;
    }

    int num_len = r - l;
    char num_str[num_len + 1];

    strncpy(num_str, l, num_len + 1);
    num_str[num_len + 1] = '\0';

    return atoi(num_str);
}

int get_gear_ratio(char *contents, struct Point p, struct Dimensions d) {
    int gear_ratio = 1;

    struct Point tl = {p.x - 1, p.y - 1};
    struct Point tt = {p.x, p.y - 1};
    struct Point tr = {p.x + 1, p.y - 1};
    struct Point ll = {p.x - 1, p.y};
    struct Point rr = {p.x + 1, p.y};
    struct Point bl = {p.x - 1, p.y + 1};
    struct Point bb = {p.x, p.y + 1};
    struct Point br = {p.x + 1, p.y + 1};

    bool in_left_bounds = p.x > 0;
    bool in_right_bounds = p.x < d.width;

    bool tl_is_digit = isdigit(c_at_pos(contents, tl, d));
    bool tt_is_digit = isdigit(c_at_pos(contents, tt, d));
    bool tr_is_digit = isdigit(c_at_pos(contents, tr, d));
    bool ll_is_digit = isdigit(c_at_pos(contents, ll, d));
    bool rr_is_digit = isdigit(c_at_pos(contents, rr, d));
    bool bl_is_digit = isdigit(c_at_pos(contents, bl, d));
    bool bb_is_digit = isdigit(c_at_pos(contents, bb, d));
    bool br_is_digit = isdigit(c_at_pos(contents, br, d));

    if (tt_is_digit) {
        gear_ratio *= -1 * get_number(contents, tt, d);
    } else {
        if (tl_is_digit && in_left_bounds) {
            gear_ratio *= -1 * get_number(contents, tl, d);
        }

        if (tr_is_digit && in_right_bounds) {
            gear_ratio *= -1 * get_number(contents, tr, d);
        }
    }

    if (ll_is_digit && in_left_bounds) {
        gear_ratio *= -1 * get_number(contents, ll, d);
    }

    if (rr_is_digit && in_right_bounds) {
        gear_ratio *= -1 * get_number(contents, rr, d);
    }

    if (bb_is_digit) {
        gear_ratio *= -1 * get_number(contents, bb, d);
    } else {
        if (bl_is_digit && in_left_bounds) {
            gear_ratio *= -1 * get_number(contents, bl, d);
        }

        if (br_is_digit && in_right_bounds) {
            gear_ratio *= -1 * get_number(contents, br, d);
        }
    }

    return gear_ratio;
}

int main() {
    int sum = 0;

    FILE *file = fopen("./day3_input.txt", "r");
    fseek(file, 0, SEEK_END);

    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *contents = malloc(size + 1);
    char *line = malloc(size + 1);

    struct Dimensions d = {0, 0};
    struct Point p = {0, 0};

    while ((line = fgets(line, size, file)) != NULL) {
        d.height++;
        d.width = strcspn(line, "\r\n");
        line[d.width] = '\0';

        strcat(contents, line);
    }

    for (p.y = 0; p.y < d.height; p.y++) {
        for (p.x = 0; p.x < d.width; p.x++) {
            char c = c_at_pos(contents, p, d);

            if (!isdigit(c)) {
                continue;
            }

            struct Point a = get_asterik(contents, p, d);

            if (a.x == -1 || a.y == -1) {
                continue;
            }

            int gear_ratio = get_gear_ratio(contents, a, d);

            if (gear_ratio > -1) {
                sum += gear_ratio;
                contents[a.y * d.width + a.x] = '.';
            }
        }
    }

    free(line);
    free(contents);
    fclose(file);

    printf("Part 2: %d\n", sum);

    return 0;
}
