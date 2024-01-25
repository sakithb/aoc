#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;

typedef unsigned long ulong;
typedef struct {
    ulong dest;
    ulong src;
    ulong offset;
} MapRange;

void parse_seeds(ulong **seeds, int *seeds_size, char **c) {
    char *s = NULL;

    while (**c != '\n') {
        if (isdigit(**c)) {
            if (isdigit(*((*c) + 1))) {
                if (s == NULL) {
                    s = *c;
                }
            } else {
                if (s != NULL) {
                    ulong num = strtoul(s, c, 10);

                    *seeds_size += 1;
                    ulong *seeds_tmp = realloc(*seeds, *seeds_size * sizeof(ulong));

                    if (seeds_tmp != NULL) {
                        *seeds = seeds_tmp;
                        (*seeds)[*seeds_size - 1] = num;
                    } else {
                        longjmp(exit_buf, 1);
                    }

                    s = NULL;
                }
            }
        }

        (*c) += 1;
    }

    (*c) -= 1;
}

void parse_map(MapRange **map, int *map_size, char **c) {
    char *n = strchr(*c, '\n');
    (*c) = strchr(n + 1, '\n');

    while (*(n + 1) != '\n') {
        char str[*c - n];
        strncpy(str, n + 1, *c - n - 1);
        str[*c - n - 1] = '\0';

        MapRange item;
        sscanf(str, "%lu %lu %lu", &item.dest, &item.src, &item.offset);

        *map_size += 1;
        MapRange *map_tmp = realloc(*map, *map_size * sizeof(MapRange));

        if (map_tmp != NULL) {
            *map = map_tmp;
            (*map)[*map_size - 1] = item;
        } else {
            longjmp(exit_buf, 1);
        }

        n = *c;
        char *tmp = strchr(*c + 1, '\n');

        if (tmp == NULL) {
            break;
        } else {
            *c = tmp;
        }
    }

    (*c) -= 1;
}

ulong src_to_dst(MapRange *map, int map_size, ulong src) {
    for (int i = 0; i < map_size; i++) {
        MapRange item = map[i];
        ulong offset = src - item.src;
        if (offset >= 0 && offset < item.offset) {
            return item.dest + offset;
        }
    }

    return src;
}

int main() {
    int seeds_size = 0;
    ulong *seeds = calloc(seeds_size, sizeof(ulong));

    int seed_to_soil_size = 0;
    MapRange *seed_to_soil = calloc(seed_to_soil_size, sizeof(MapRange));

    int soil_to_fertilizer_size = 0;
    MapRange *soil_to_fertilizer = calloc(soil_to_fertilizer_size, sizeof(MapRange));

    int fertilizer_to_water_size = 0;
    MapRange *fertilizer_to_water = calloc(fertilizer_to_water_size, sizeof(MapRange));

    int water_to_light_size = 0;
    MapRange *water_to_light = calloc(water_to_light_size, sizeof(MapRange));

    int light_to_temperature_size = 0;
    MapRange *light_to_temperature = calloc(light_to_temperature_size, sizeof(MapRange));

    int temperature_to_humidity_size = 0;
    MapRange *temperature_to_humidity = calloc(temperature_to_humidity_size, sizeof(MapRange));

    int humidity_to_location_size = 0;
    MapRange *humidity_to_location = calloc(humidity_to_location_size, sizeof(MapRange));

    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    ulong fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = calloc(fsize + 1, sizeof(char));
    fread(input, sizeof(char), fsize + 1, file);

    fclose(file);

    if (setjmp(exit_buf) != 0) {
        free(input);

        free(seeds);
        free(seed_to_soil);
        free(soil_to_fertilizer);
        free(fertilizer_to_water);
        free(water_to_light);
        free(light_to_temperature);
        free(temperature_to_humidity);
        free(humidity_to_location);

        return 0;
    }

    char *n = input;
    char *c = input;

    while (*c != '\0') {
        if (*c == '\n') {
            n = c + 1;
        } else if (*c == ':') {
            char line[c - n + 1];
            strncpy(line, n, c - n);
            line[c - n] = '\0';

            if (strncmp("seeds", line, 5) == 0) {
                parse_seeds(&seeds, &seeds_size, &c);
            } else {
                if (strncmp("seed-", line, 5) == 0) {
                    parse_map(&seed_to_soil, &seed_to_soil_size, &c);
                } else if (strncmp("soil-", line, 5) == 0) {
                    parse_map(&soil_to_fertilizer, &soil_to_fertilizer_size, &c);
                } else if (strncmp("ferti", line, 5) == 0) {
                    parse_map(&fertilizer_to_water, &fertilizer_to_water_size, &c);
                } else if (strncmp("water", line, 5) == 0) {
                    parse_map(&water_to_light, &water_to_light_size, &c);
                } else if (strncmp("light", line, 5) == 0) {
                    parse_map(&light_to_temperature, &light_to_temperature_size, &c);
                } else if (strncmp("tempe", line, 5) == 0) {
                    parse_map(&temperature_to_humidity, &temperature_to_humidity_size, &c);
                } else if (strncmp("humid", line, 5) == 0) {
                    parse_map(&humidity_to_location, &humidity_to_location_size, &c);
                }
            }
        }

        c++;
    }

    ulong min_location = 0;

    for (int i = 0; i < seeds_size; i++) {
        ulong seed = seeds[i];
        ulong soil = src_to_dst(seed_to_soil, seed_to_soil_size, seed);
        ulong fertilizer = src_to_dst(soil_to_fertilizer, soil_to_fertilizer_size, soil);
        ulong water = src_to_dst(fertilizer_to_water, fertilizer_to_water_size, fertilizer);
        ulong light = src_to_dst(water_to_light, water_to_light_size, water);
        ulong temperature = src_to_dst(light_to_temperature, light_to_temperature_size, light);
        ulong humidity = src_to_dst(temperature_to_humidity, temperature_to_humidity_size, temperature);
        ulong location = src_to_dst(humidity_to_location, humidity_to_location_size, humidity);

        if (min_location == 0 || location < min_location) {
            min_location = location;
        }
    }

    printf("Min location: %lu\n", min_location);

    longjmp(exit_buf, 1);
}
