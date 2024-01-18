#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const int RED_CUBES = 12;
const int GREEN_CUBES = 13;
const int BLUE_CUBES = 14;

struct DrawSet {
    long red;
    long green;
    long blue;
};

struct DrawSet parse_set(char *setstr, int setlen) {
    long red = 0;
    long green = 0;
    long blue = 0;
    
    int lastnostart = -1;

    for (int i = 0; i < setlen; i++) {
        char letter = setstr[i];

        if (lastnostart == -1 && letter >= 48 && letter <= 57) {
            lastnostart = i;
        } else if (lastnostart != -1 && letter == ' ') {
            char no[2];
            strncpy(no, setstr + lastnostart, 2);

            char next_letter = setstr[i + 1];
            if (next_letter == 'r') {
                red = strtol(no, (char **)NULL, 10);
            } else if (next_letter == 'g') {
                green = strtol(no, (char **)NULL, 10);
            } else if (next_letter == 'b') {
                blue = strtol(no, (char **)NULL, 10);
            }


            lastnostart = -1;
        }         
    }

    struct DrawSet res = {red, green, blue};
    return res;
}

int main()
{
    FILE *input = fopen("./day2_input.txt", "r");
    char line[255];
    long sum = 0;

    while(fgets(line, 255, input) != NULL) {
        int curr_start = -1;
        char *id_slc = line + 4;
        long id = -1;

        long maxred = 0;
        long maxgreen = 0;
        long maxblue = 0;

        for (int i = 0; i < strlen(line); i++) {
            char letter = line[i];

            if (id == -1 && letter == ':') {
                char idstr[i - 4];
                strncpy(idstr, id_slc, i - 4);

                id = strtol(idstr, (char **)NULL, 10);
                curr_start = i + 1;
            }


            if (id != -1 && curr_start != -1 && (letter == ';' || letter == '\n')) {
                struct DrawSet set = parse_set(line + curr_start, i - curr_start);

                if (set.red > maxred) {
                    maxred = set.red;
                }
                
                if (set.green > maxgreen) {
                    maxgreen = set.green;
                }
                
                if (set.blue > maxblue) {
                    maxblue = set.blue;
                }

                curr_start = i + 1;
            }
        }
        
        sum += maxred * maxgreen * maxblue;
        
    }

    fclose(input);
    printf("%ld\n", sum);

    return 0;
}
