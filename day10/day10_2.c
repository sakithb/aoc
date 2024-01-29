#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

typedef enum {
    NONE = 0,
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    TOP = 1 << 2,
    BOTTOM = 1 << 3,
} Direction;

typedef enum {
    UNKNOWN,
    VERTICAL,
    HORIZONTAL,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_WEST,
    SOUTH_EAST,
    GROUND,
} CellType;

typedef struct {
    CellType type;
    Direction dirs;

    bool loop_start;
    bool loop_bound;
    bool loop_in;

    int x;
    int y;
} Cell;

typedef struct {
    int cols;
    int rows;

    int top_most;
    int right_most;
    int bottom_most;
    int left_most;

    Cell **data;
} Matrix;

CellType get_type_from_dirs(Direction dirs) {
    if (dirs == (TOP | RIGHT)) {
        return NORTH_EAST;
    } else if (dirs == (TOP | LEFT)) {
        return NORTH_WEST;
    } else if (dirs == (BOTTOM | LEFT)) {
        return SOUTH_WEST;
    } else if (dirs == (BOTTOM | RIGHT)) {
        return SOUTH_EAST;
    } else if (dirs == (TOP | BOTTOM)) {
        return VERTICAL;
    } else if (dirs == (LEFT | RIGHT)) {
        return HORIZONTAL;
    } else {
        return GROUND;
    }
}

bool scan_bounds(int row, int col, Direction dir, Matrix *matrix) {
    Cell *current = *(matrix->data + row) + col;

    if (dir != NONE && current->loop_start) {
        current->dirs = dir;
        current->loop_bound = true;
        return true;
    }

    if (row < matrix->top_most) {
        matrix->top_most = row;
    }

    if (row > matrix->bottom_most) {
        matrix->bottom_most = row;
    }

    if (col > matrix->right_most) {
        matrix->right_most = col;
    }

    if (col < matrix->left_most) {
        matrix->left_most = col;
    }

    if (dir != LEFT && col > 0 && current->dirs & LEFT) {
        Cell *left = *(matrix->data + row) + col - 1;

        if (left->dirs & RIGHT && scan_bounds(row, col - 1, RIGHT, matrix)) {
            if (current->loop_start) {
                current->dirs |= LEFT;
                current->type = get_type_from_dirs(current->dirs);
            }

            current->loop_bound = true;
            return true;
        }
    }

    if (dir != RIGHT && col < matrix->cols && current->dirs & RIGHT) {
        Cell *right = *(matrix->data + row) + col + 1;

        if (right->dirs & LEFT && scan_bounds(row, col + 1, LEFT, matrix)) {
            if (current->loop_start) {
                current->dirs |= RIGHT;
                current->type = get_type_from_dirs(current->dirs);
            }

            current->loop_bound = true;
            return true;
        }
    }

    if (dir != TOP && row > 0 && current->dirs & TOP) {
        Cell *top = *(matrix->data + (row - 1)) + col;

        if (top->dirs & BOTTOM && scan_bounds(row - 1, col, BOTTOM, matrix)) {
            if (current->loop_start) {
                current->dirs |= TOP;
                current->type = get_type_from_dirs(current->dirs);
            }

            current->loop_bound = true;
            return true;
        }
    }

    if (dir != BOTTOM && row < matrix->rows && current->dirs & BOTTOM) {
        Cell *bottom = *(matrix->data + (row + 1)) + col;

        if (bottom->dirs & TOP && scan_bounds(row + 1, col, TOP, matrix)) {
            if (current->loop_start) {
                current->dirs |= BOTTOM;
                current->type = get_type_from_dirs(current->dirs);
            }

            current->loop_bound = true;
            return true;
        }
    }

    return false;
}

bool scan_in_loop(int row, int col, Matrix *matrix) {
    int bounds = 0;

    CellType edge_start = -1;

    while (col < matrix->cols) {
        Cell *current = *(matrix->data + row) + col;

        if (current->loop_bound && current->type != HORIZONTAL) {
            if (current->type == VERTICAL) {
                bounds++;
            } else {
                if (edge_start == -1) {
                    if (current->type == NORTH_EAST) {
                        edge_start = NORTH_WEST;
                    } else if (current->type == SOUTH_EAST) {
                        edge_start = SOUTH_WEST;
                    }
                } else {
                    if (edge_start != current->type) {
                        bounds++;
                    }

                    edge_start = -1;
                }
            }
        }

        col++;
    }

    return bounds % 2 == 1;
}

int main(int argc, char **argv) {
    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    ll fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(fsize + 1);
    fread(input, 1, fsize, file);
    fclose(file);

    Matrix matrix;
    matrix.rows = 0;
    matrix.cols = 0;

    for (char *c = input; *c != '\0'; c++) {
        if (*c == '\n') {
            if (matrix.cols == 0) {
                matrix.cols = c - input;
            }

            matrix.rows++;
        }
    }

    matrix.data = calloc(matrix.rows, sizeof(Cell *));

    matrix.bottom_most = 0;
    matrix.left_most = matrix.cols - 1;
    matrix.top_most = matrix.rows - 1;
    matrix.right_most = 0;

    int start_row = 0;
    int start_col = 0;

    for (int y = 0; y < matrix.rows; y++) {
        matrix.data[y] = calloc(matrix.cols, sizeof(Cell));

        for (int x = 0; x < matrix.cols; x++) {
            char *c = input + (y * (matrix.cols + 1) + x);
            matrix.data[y][x].x = x;
            matrix.data[y][x].y = y;

            switch (*c) {
                case '|':
                    matrix.data[y][x].type = VERTICAL;
                    matrix.data[y][x].dirs = TOP | BOTTOM;
                    break;
                case '-':
                    matrix.data[y][x].type = HORIZONTAL;
                    matrix.data[y][x].dirs = LEFT | RIGHT;
                    break;
                case 'L':
                    matrix.data[y][x].type = NORTH_EAST;
                    matrix.data[y][x].dirs = TOP | RIGHT;
                    break;
                case 'J':
                    matrix.data[y][x].type = NORTH_WEST;
                    matrix.data[y][x].dirs = TOP | LEFT;
                    break;
                case '7':
                    matrix.data[y][x].type = SOUTH_WEST;
                    matrix.data[y][x].dirs = BOTTOM | LEFT;
                    break;
                case 'F':
                    matrix.data[y][x].type = SOUTH_EAST;
                    matrix.data[y][x].dirs = BOTTOM | RIGHT;
                    break;
                case '.':
                    matrix.data[y][x].type = GROUND;
                    matrix.data[y][x].dirs = NONE;
                    break;
                case 'S':
                    matrix.data[y][x].type = UNKNOWN;
                    matrix.data[y][x].dirs = TOP | RIGHT | BOTTOM | LEFT;
                    matrix.data[y][x].loop_start = true;
                    start_row = y;
                    start_col = x;
                    break;
                default:
                    printf("Unknown cell type: %c\n", *c);
                    return 1;
            }
        }
    }

    scan_bounds(start_row, start_col, NONE, &matrix);
    int count = 0;

    for (int y = matrix.top_most; y <= matrix.bottom_most; y++) {
        for (int x = matrix.left_most; x <= matrix.right_most; x++) {
            if (matrix.data[y][x].loop_bound) {
                continue;
            }

            if (scan_in_loop(y, x, &matrix)) {
                matrix.data[y][x].loop_in = true;
                count++;
            }
        }
    }

    if (argc > 1 && strcmp(argv[1], "print") == 0) {
        for (int y = matrix.top_most; y <= matrix.bottom_most; y++) {
            for (int x = matrix.left_most; x <= matrix.right_most; x++) {
                Cell *cell = &matrix.data[y][x];
                if (cell->loop_bound) {
                    printf("\033[0;32m");
                } else if (cell->loop_in) {
                    printf("\033[0;31m");
                }

                if (x == start_col && y == start_row) {
                    printf("\033[0;34m");
                }

                switch (cell->type) {
                    case VERTICAL:
                        printf("║");
                        break;
                    case HORIZONTAL:
                        printf("═");
                        break;
                    case NORTH_EAST:
                        printf("╚");
                        break;
                    case NORTH_WEST:
                        printf("╝");
                        break;
                    case SOUTH_WEST:
                        printf("╗");
                        break;
                    case SOUTH_EAST:
                        printf("╔");
                        break;
                    case GROUND:
                        printf(".");
                        break;
                    case UNKNOWN:
                        printf("?");
                        break;
                }

                printf("\033[0m");
            }

            printf("\n");
        }
    }

    printf("Result: %d\n", count);

    for (int y = 0; y < matrix.rows; y++) {
        free(matrix.data[y]);
    }

    free(matrix.data);
    free(input);
    return 0;
}
