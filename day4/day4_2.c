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
			if (!(c == 'A' && x > 0 && y > 0 && x < (w - 1) && y < (h - 1))) continue;

			char tl = grid[y - 1][x - 1];
			char tr = grid[y - 1][x + 1];
			char bl = grid[y + 1][x - 1];
			char br = grid[y + 1][x + 1];

			int tlbr = (tl == 'M' && br == 'S') ||(tl == 'S' && br == 'M');
			int trbl = (tr == 'M' && bl == 'S') ||(tr == 'S' && bl == 'M');

			if (tlbr && trbl) {
				n++;
			}
		}
	}

	printf("%d\n", n);

	return 0;
}
