#include <stdio.h>

int main() {
	FILE *f = fopen("day4_input.txt", "r");

	int w = 0;
	while (fgetc(f) != '\n') w++;

	fseek(f, 0, SEEK_END);
	int h = ftell(f) / (w + 1);

	char grid[h][w];

	fseek(f, 0, SEEK_SET);

	char c;
	int x = 0;
	int y = 0;

	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') {
			y++;
			x = 0;
			continue;
		}

		grid[y][x] = c;
		x++;
	}

	fclose(f);

	int n = 0;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			char c = grid[y][x];
			if (c != 'X') continue;

			int right_valid = w - x >= 4;
			int left_valid = x >= 3;
			int top_valid = y >= 3;
			int bottom_valid = h - y >= 4;

			if (
				right_valid &&
				grid[y][x + 1] == 'M' &&
				grid[y][x + 2] == 'A' &&
				grid[y][x + 3] == 'S'
			) {
				n++;
			}

			if (
				left_valid &&
				grid[y][x - 1] == 'M' &&
				grid[y][x - 2] == 'A' &&
				grid[y][x - 3] == 'S'
			) {
				n++;
			}

			if (
				top_valid &&
				grid[y - 1][x] == 'M' &&
				grid[y - 2][x] == 'A' &&
				grid[y - 3][x] == 'S'
			) {
				n++;
			}

			if (
				bottom_valid &&
				grid[y + 1][x] == 'M' &&
				grid[y + 2][x] == 'A' &&
				grid[y + 3][x] == 'S'
			) {
				n++;
			}

			if (
				top_valid &&
				left_valid &&
				grid[y - 1][x - 1] == 'M' &&
				grid[y - 2][x - 2] == 'A' &&
				grid[y - 3][x - 3] == 'S'
			) {
				n++;
			}

			if (
				top_valid &&
				right_valid &&
				grid[y - 1][x + 1] == 'M' &&
				grid[y - 2][x + 2] == 'A' &&
				grid[y - 3][x + 3] == 'S'
			) {
				n++;
			}

			if (
				bottom_valid &&
				right_valid &&
				grid[y + 1][x + 1] == 'M' &&
				grid[y + 2][x + 2] == 'A' &&
				grid[y + 3][x + 3] == 'S'
			) {
				n++;
			}

			if (
				bottom_valid &&
				left_valid &&
				grid[y + 1][x - 1] == 'M' &&
				grid[y + 2][x - 2] == 'A' &&
				grid[y + 3][x - 3] == 'S'
			) {
				n++;
			}
		}
	}

	printf("%d\n", n);

	return 0;
}
