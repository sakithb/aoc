
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

bool is_substr(char *str, char *sub, int idx) {
    for (int i = 0; i < strlen(sub); i++) {
        if ((idx + i) > (strlen(str) - 1)) {
            return false;
        }

        char subchar = sub[i];
        char strchar = str[idx + i];

        if (subchar != strchar) {
            return false;
        }
    }

    return true;
}

int main()
{
	FILE* input = fopen("./day1_input.txt", "r");
    char line[255];
    long sum = 0;

    while (fgets(line, 255, input) != NULL) {
        char first = '\0';
        char last = '\0';

        for (int i = 0; i <= strlen(line); i++) {
            char letter = line[i];

            if (letter >= 48 && letter <= 57) {
                if (first == '\0') {
                    first = letter;
                    last = letter;
                } else {
                    last = letter;
                }
            } else if (letter >= 65 && letter <= 122) {
                char num = '\0';

                switch(letter) {
                    case 'o':
                        if (is_substr(line, "one", i)) {
                            num = '1';
                        }
                        break;
                    case 't':
                        if (is_substr(line, "two", i)) {
                            num = '2';
                        } else if (is_substr(line, "three", i)) {
                            num = '3';
                        }
                        break;
                    case 'f':
                        if (is_substr(line, "four", i)) {
                            num = '4';
                        } else if (is_substr(line, "five", i)) {
                            num = '5';
                        }
                        break;
                    case 's':
                        if (is_substr(line, "six", i)) {
                            num = '6';
                        } else if (is_substr(line, "seven", i)) {
                            num = '7';
                        }
                        break;
                    case 'e':
                        if (is_substr(line, "eight", i)) {
                            num = '8';
                        }
                        break;
                    case 'n':
                        if (is_substr(line, "nine", i)) {
                            num = '9';
                        }
                        break;
                }

                if (num != '\0') {
                    if (first == '\0') {
                        first = num;
                        last = num;
                    } else {
                        last = num;
                    }
                }
            }
        }

        char str[2];
        sprintf(str, "%c%c", first, last);

        sum += strtol(str, (char **)NULL, 10);
    };

	fclose(input);
    printf("%ld\n", sum);

	return 0;
}
