#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

typedef enum {
    NONE = 1 << 0,
    LEFT = 1 << 1,
    RIGHT = 1 << 2,
    TOP = 1 << 3,
    BOTTOM = 1 << 4,
} Direction;

typedef enum {
    VERTICAL,
    HORIZONTAL,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_WEST,
    SOUTH_EAST,
    GROUND,
    START,
} CellType;

typedef struct {
    CellType type;
    Direction dirs;
    int x;
    int y;
} Cell;

typedef struct {
    int cols;
    int rows;
    Cell **data;
} Map;

int walk(int row, int col, int steps, Direction dir, Map *matrix) {
    Cell *current = *(matrix->data + row) + col;

    if (current->type == START) {
        if (steps > 0) {
            return steps;
        }
    } else {
        steps++;
    }

    if (dir != LEFT && col > 0 && current->dirs & LEFT) {
        Cell *left = *(matrix->data + row) + col - 1;

        if (left->dirs & RIGHT) {
            int tmp_steps = walk(row, col - 1, steps, RIGHT, matrix);

            if (tmp_steps > 0) {
                return tmp_steps;
            }
        }
    }

    if (dir != RIGHT && col < matrix->cols && current->dirs & RIGHT) {
        Cell *right = *(matrix->data + row) + col + 1;

        if (right->dirs & LEFT) {
            int tmp_steps = walk(row, col + 1, steps, LEFT, matrix);

            if (tmp_steps > 0) {
                return tmp_steps;
            }
        }
    }

    if (dir != TOP && row > 0 && current->dirs & TOP) {
        Cell *top = *(matrix->data + (row - 1)) + col;

        if (top->dirs & BOTTOM) {
            int tmp_steps = walk(row - 1, col, steps, BOTTOM, matrix);

            if (tmp_steps > 0) {
                return tmp_steps;
            }
        }
    }

    if (dir != BOTTOM && row < matrix->rows && current->dirs & BOTTOM) {
        Cell *bottom = *(matrix->data + (row + 1)) + col;

        if (bottom->dirs & TOP) {
            int tmp_steps = walk(row + 1, col, steps, TOP, matrix);

            if (tmp_steps > 0) {
                return tmp_steps;
            }
        }
    }

    return 0;
}

int main() {

    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    ll fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(fsize + 1);
    fread(input, 1, fsize, file);
    fclose(file);

    Map matrix;
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
                    matrix.data[y][x].type = START;
                    matrix.data[y][x].dirs = TOP | RIGHT | BOTTOM | LEFT;
                    start_row = y;
                    start_col = x;
                    break;
                default:
                    printf("Unknown cell type: %c\n", *c);
                    return 1;
            }
        }
    }

    int steps = walk(start_row, start_col, 0, NONE, &matrix);
    steps = steps / 2 + steps % 2;
    printf("%d\n", steps);

    for (int y = 0; y < matrix.rows; y++) {
        free(matrix.data[y]);
    }

    free(matrix.data);
    free(input);
    return 0;
}
