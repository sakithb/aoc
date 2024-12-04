#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_line(char *line, int index) {
	int is_unsafe = 0;
	int type;
	int left = -1;
	int right = -1;

	char *start = line;
	char *end = line;

	char *tmp;

	int count = 0;

	while (1) {
		if (*end == ' ' || *end == '\n') {
			if (count == index) {
				start = end + 1;
				count++;

				if (*end == '\n') {
					break;
				} else {
					end++;
					continue;
				}
			}

			int num_len = end - start;
			char num_str[num_len + 1];

			strncpy(num_str, start, num_len);
			num_str[num_len] = 0;

			if (left == -1) {
				left = strtol(num_str, &tmp, 10);
				//printf("%d ", left);
			} else if (right == -1) {
				right = strtol(num_str, &tmp, 10);
				//printf("%d ", right);

				if (left < right) {
					type = 1;
				} else {
					type = -1;
				}
			} else {
				left = right;
				right = strtol(num_str, &tmp, 10);
				//printf("%d ", right);

				if (type == 1 && left > right) is_unsafe = 1;
				if (type == -1 && left < right) is_unsafe = 1;
			}

			if (left == right) is_unsafe = 1;
			if (right != -1 && left - right > 3 || left - right < -3) is_unsafe = 1;

			start = end + 1;
			count++;

			if (*end == '\n') {
				break;
			}
		};

		end++;
	}


	if (is_unsafe) {
		//printf("---\n");
		return count;
	} else {
		//printf("\n");
		return 0;
	}
}

int main(void) {
	FILE *f = fopen("day2_input.txt", "r");

	int safe_reports = 0;
	char line[1000];

	while (fgets(line, 1000, f) != NULL) {
		int count = check_line(line, -1);
		int is_unsafe = count > 0;

		// Part 2
		while(count != 0) {
			int nc = check_line(line, count - 1);
			if (nc == 0) {
				is_unsafe = 0;
				break;
			}

			count--;
		}
		// Part 2

		if (!is_unsafe) {
			safe_reports++;
		}
	}

	fclose(f);

	printf("%d\n", safe_reports);

	return 0;
};
