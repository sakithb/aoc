#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	FILE *f = fopen("day3_input.txt", "r");

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *in = malloc(size);
	fread(in, 1, size - 1, f);

	fclose(f);

	char *p = in;
	char *t;

	int sum = 0;

	int skip = 0;

	while (p != in + (size - 1)) {
		if (
			*p == 'm' &&
			*(p + 1) == 'u' &&
			*(p + 2) == 'l' &&
			*(p + 3) == '('
		) {
			if (skip) {
				p++;
				continue;
			}

			char *num1_start = p + 4;

			char *comma = strchr(num1_start, ',');
			if (comma == NULL) {
				p++;
				continue;
			}

			char *end_bracket = strchr(comma, ')');
			if (end_bracket == NULL) {
				p++;
				continue;
			}

			char *num2_start = comma + 1;

			int num1_len = comma - num1_start;
			char num1_str[num1_len];
			strncpy(num1_str, num1_start, num1_len);
			num1_str[num1_len] = 0;

			int num1 = 0;
			int m1 = 1;

			while (num1_len > 0) {
				int d = num1_str[num1_len - 1] - 48;
				if (d < 0 || d > 9) break;

				num1 += m1 * d;
				m1 *= 10;
				num1_len--;
			}

			if (num1_len > 0) {
				p++;
				continue;
			}

			int num2_len = end_bracket - num2_start;
			char num2_str[num2_len];
			strncpy(num2_str, num2_start, num2_len);

			int num2 = 0;
			int m2 = 1;

			while (num2_len > 0) {
				int d = num2_str[num2_len - 1] - 48;
				if (d < 0 || d > 9) break;

				num2 += m2 * d;
				m2 *= 10;
				num2_len--;
			}

			if (num2_len > 0) {
				p++;
				continue;
			}

			printf("%d * %d\n", num1, num2);

			sum += num1 * num2;
			p = end_bracket + 1;
		} else if (
			*p == 'd' &&
			*(p + 1) == 'o' &&
			*(p + 2) == 'n' &&
			*(p + 3) == '\'' &&
			*(p + 4) == 't' &&
			*(p + 5) == '(' &&
			*(p + 6) == ')'
		) {
			skip = 1;
			p += 7;
		} else if (
			*p == 'd' &&
			*(p + 1) == 'o' &&
			*(p + 2) == '(' &&
			*(p + 3) == ')'
		) {
			skip = 0;
			p += 4;
		} else {
			p++;
		}
	}

	free(in);

	printf("%d\n", sum);

	return 0;
}
