#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b) {
	int ai = *(const int*)a;
    int bi = *(const int*)b;
	return (ai > bi) - (ai < bi);
}

int main(void) {

	FILE *f = fopen("day1_input.txt", "r");

	fseek(f, 0, SEEK_END);
	long s = ftell(f);
	char *b = malloc(s);
	fseek(f, 0, SEEK_SET);
	fread(b, 1, s, f);

	fclose(f);

	int lines = s / 14;

	int l[lines];
	int r[lines];

	for (int i = 0; i < lines; i++) {
		char left[6];
		strncpy(left, b + (i * 14), 5);
		left[5] = 0;

		char rght[14];
		strncpy(rght, b + (i * 14) + 8, 5);
		rght[5] = 0;

		char *e;

		l[i] = strtol(left, &e, 10);
		r[i] = strtol(rght, &e, 10);
	}

	qsort(l, lines, sizeof(int), cmp);
	qsort(r, lines, sizeof(int), cmp);

	int dist[lines];

	for (int i = 0; i < lines; i++) {
		dist[i] = l[i] - r[i];
		if (dist[i] < 0) {
			dist[i] *= -1;
		}
	}

	int sum = 0;

	for (int i = 0; i < lines; i++) {
		sum += dist[i];
	}

	printf("%d\n", sum);

	int scr = 0;

	for (int i = 0; i < lines; i++) {
		int n = 0;
		for (int j = 0; j < lines; j++) {
			if (l[i] == r[j]) {
				n++;
			}
		}
		scr += l[i] * n;
	}

	printf("%d\n", scr);

	free(b);

	return 0;
};
