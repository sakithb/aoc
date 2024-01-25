#include <ctype.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;

typedef unsigned long ulong;

typedef struct {
    ulong src;
    ulong dest;
    ulong offset;
} MapRange;

typedef struct {
    MapRange *ranges;
    int size;
} Map;

typedef struct {
    Map *seed_to_soil;
    Map *soil_to_fertilizer;
    Map *fertilizer_to_water;
    Map *water_to_light;
    Map *light_to_temperature;
    Map *temperature_to_humidity;
    Map *humidity_to_location;
} Maps;

Map *init_map() {
    Map *mapitems = malloc(sizeof(Map));
    mapitems->size = 0;
    mapitems->ranges = calloc(mapitems->size, sizeof(MapRange));

    return mapitems;
}

void free_map(Map *map) {
    free(map->ranges);
    free(map);
}

void resize_map(Map *map, int new_size) {
    MapRange *map_tmp = realloc(map->ranges, new_size * sizeof(MapRange));

    if (map_tmp != NULL) {
        map->ranges = map_tmp;
        map->size = new_size;
    } else {
        longjmp(exit_buf, 1);
    }
}

int cmp_map_range(const void *a, const void *b) {
    MapRange *range_a = (MapRange *)a;
    MapRange *range_b = (MapRange *)b;

    if (range_a->src < range_b->src) {
        return -1;
    } else if (range_a->src > range_b->src) {
        return 1;
    } else {
        return 0;
    }
}

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

void parse_map(Map *map, char **c) {
    char *n = strchr(*c, '\n');
    (*c) = strchr(n + 1, '\n');

    while (*(n + 1) != '\n') {
        char str[*c - n];
        strncpy(str, n + 1, *c - n - 1);
        str[*c - n - 1] = '\0';

        MapRange range;
        sscanf(str, "%lu %lu %lu", &range.dest, &range.src, &range.offset);

        resize_map(map, map->size + 1);
        map->ranges[map->size - 1] = range;

        n = *c;
        char *tmp = strchr(*c + 1, '\n');

        if (tmp == NULL) {
            break;
        } else {
            *c = tmp;
        }
    }

    qsort(map->ranges, map->size, sizeof(MapRange), cmp_map_range);

    for (int i = map->size - 1; i >= 0; i--) {
        MapRange range = map->ranges[i];
        MapRange new_range;

        if (i == 0) {
            continue;
        } else {
            MapRange prev_range = map->ranges[i - 1];

            if (range.src > prev_range.src + prev_range.offset) {
                new_range.src = prev_range.src + prev_range.offset;
                new_range.offset = range.src - new_range.src;
            } else {
                continue;
            }
        }

        new_range.dest = new_range.src;
        resize_map(map, map->size + 1);
        map->ranges[map->size - 1] = new_range;
    }

    qsort(map->ranges, map->size, sizeof(MapRange), cmp_map_range);

    (*c) -= 1;
}

void find_min_location(ulong start, ulong end, ulong *min_location, Map *map, Maps *maps) {
    if (map == NULL) {
        if (*min_location == 0 || start < *min_location) {
            *min_location = start;
        }

        return;
    }

    Map *next_map;

    if (map == maps->seed_to_soil) {
        next_map = maps->soil_to_fertilizer;
    } else if (map == maps->soil_to_fertilizer) {
        next_map = maps->fertilizer_to_water;
    } else if (map == maps->fertilizer_to_water) {
        next_map = maps->water_to_light;
    } else if (map == maps->water_to_light) {
        next_map = maps->light_to_temperature;
    } else if (map == maps->light_to_temperature) {
        next_map = maps->temperature_to_humidity;
    } else if (map == maps->temperature_to_humidity) {
        next_map = maps->humidity_to_location;
    } else {
        next_map = NULL;
    }

    for (int i = 0; i < map->size; i++) {
        MapRange *range = &map->ranges[i];

        ulong range_start = range->src;
        ulong range_end = range->src + range->offset;

        if (start < range_end && end > range_start) {
            ulong new_start = range_start > start ? range->dest : range->dest + (start - range_start);
            ulong new_end = range_end > end ? range->dest + (end - range_start) : range->dest + range->offset;

            find_min_location(new_start, new_end, min_location, next_map, maps);
        }

        if (i == 0 && start < range_start) {
            find_min_location(start, end < range_start ? end : range_start, min_location, next_map, maps);
        } else if (i == map->size - 1 && end > range_end) {
            find_min_location(start < range_end ? range_end : start, end, min_location, next_map, maps);
        }
    }
}

int main() {
    int seeds_size = 0;
    ulong *seeds = calloc(seeds_size, sizeof(ulong));

    Maps *maps = malloc(sizeof(Maps));
    maps->seed_to_soil = init_map();
    maps->soil_to_fertilizer = init_map();
    maps->fertilizer_to_water = init_map();
    maps->water_to_light = init_map();
    maps->light_to_temperature = init_map();
    maps->temperature_to_humidity = init_map();
    maps->humidity_to_location = init_map();

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

        free_map(maps->seed_to_soil);
        free_map(maps->soil_to_fertilizer);
        free_map(maps->fertilizer_to_water);
        free_map(maps->water_to_light);
        free_map(maps->light_to_temperature);
        free_map(maps->temperature_to_humidity);
        free_map(maps->humidity_to_location);
        free(maps);

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
                    parse_map(maps->seed_to_soil, &c);
                } else if (strncmp("soil-", line, 5) == 0) {
                    parse_map(maps->soil_to_fertilizer, &c);
                } else if (strncmp("ferti", line, 5) == 0) {
                    parse_map(maps->fertilizer_to_water, &c);
                } else if (strncmp("water", line, 5) == 0) {
                    parse_map(maps->water_to_light, &c);
                } else if (strncmp("light", line, 5) == 0) {
                    parse_map(maps->light_to_temperature, &c);
                } else if (strncmp("tempe", line, 5) == 0) {
                    parse_map(maps->temperature_to_humidity, &c);
                } else if (strncmp("humid", line, 5) == 0) {
                    parse_map(maps->humidity_to_location, &c);
                }
            }
        }

        c++;
    }

    ulong min_location = 0;

    for (int i = 1; i < seeds_size; i += 2) {
        ulong range_start = seeds[i - 1];
        ulong offset = seeds[i] - 1;

        find_min_location(range_start, range_start + offset, &min_location, maps->seed_to_soil, maps);
    }

    printf("Min location: %lu\n", min_location);

    longjmp(exit_buf, 1);
}
