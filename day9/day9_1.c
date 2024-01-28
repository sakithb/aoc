#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int size;
    int capacity;
    int *arr;
} Array;

void add_to_array(Array *arr, int value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->arr = realloc(arr->arr, arr->capacity * sizeof(int));
    }

    arr->arr[arr->size++] = value;
}

Array parse_predictions(char *line, int line_length) {
    Array predictions;
    predictions.size = 0;
    predictions.capacity = 10;
    predictions.arr = calloc(predictions.capacity, sizeof(int));

    char *n = line;
    char *c = n + strspn(n, "-0123456789");

    while (c - line <= line_length) {
        char *str = calloc((c - n) + 2, sizeof(char));
        strncpy(str, n, (c - n) + 1);

        int value = atoi(str);
        add_to_array(&predictions, value);

        n = c + 1;
        c = n + strspn(n, "-0123456789");
    }

    return predictions;
}

int get_last_diff_sum(Array *predictions) {
    Array diffs;
    diffs.capacity = predictions->size - 1;
    diffs.size = 0;
    diffs.arr = calloc(diffs.capacity, sizeof(int));

    int are_all_diffs_zero = 1;

    for (int i = 0; i < predictions->size - 1; i++) {
        int diff = predictions->arr[i + 1] - predictions->arr[i];
        add_to_array(&diffs, diff);

        if (diff != 0) {
            are_all_diffs_zero = 0;
        }
    }

    int result;

    if (are_all_diffs_zero == 0 && diffs.size > 1) {
        result = get_last_diff_sum(&diffs) + predictions->arr[predictions->size - 1];
    } else {
        result = predictions->arr[predictions->size - 1] + diffs.arr[diffs.size - 1];
    }

    free(diffs.arr);
    return result;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    int fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = calloc(fsize + 1, sizeof(char));
    fread(input, sizeof(char), fsize, file);
    fclose(file);

    char *n = input;
    char *c = strchr(input, '\n');

    long long result = 0;

    while (*c != '\0') {
        if (*c != '\n') {
            c++;
            continue;
        }

        Array predictions = parse_predictions(n, c - n);

        int next_prediction = get_last_diff_sum(&predictions);
        result += next_prediction;

        free(predictions.arr);

        n = c + 1;
        c++;
    }

    printf("Result: %lld\n", result);
    free(input);
    return 0;
}
