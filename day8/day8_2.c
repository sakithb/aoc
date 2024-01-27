#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;

typedef struct Node Node;
typedef struct Network Network;
typedef struct Path Path;
typedef long long ll;

struct Node {
    char *name;
    Node *left;
    Node *right;
};

struct Network {
    int size;
    int capacity;
    Node **nodes;
};

struct Path {
    Node *current;
    int steps;
};

ll gcd(ll a, ll b) {
    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}

void parse_node_name(char *line, Network *network) {
    Node *node = malloc(sizeof(Node));
    node->name = calloc(4, sizeof(char));
    node->left = NULL;
    node->right = NULL;

    strncpy(node->name, line, 3);

    if (network->size == network->capacity) {
        network->capacity *= 2;
        Node **tmp_nodes = realloc(network->nodes, network->capacity * sizeof(Node *));

        if (tmp_nodes == NULL) {
            longjmp(exit_buf, 1);
        } else {
            network->nodes = tmp_nodes;
        }
    }

    network->nodes[network->size] = node;
    network->size += 1;
}

void parse_node_data(char *line, Node *node, Network *network) {
    char *left_s = line + 7;
    char *right_s = line + 12;

    for (int i = 0; i < network->size; i++) {
        Node *n = network->nodes[i];
        if (strncmp(n->name, left_s, 3) == 0) {
            node->left = n;
        }

        if (strncmp(n->name, right_s, 3) == 0) {
            node->right = n;
        }

        if (node->left != NULL && node->right != NULL) {
            break;
        }
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    fseek(file, 0, SEEK_END);

    int fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = calloc(fsize + 1, sizeof(char));
    fread(input, sizeof(char), fsize, file);
    fclose(file);

    int instructions_size = strcspn(input, "\n");
    int map_start = instructions_size + 2;

    Network network;
    network.size = 0;
    network.capacity = 10;
    network.nodes = calloc(network.capacity, sizeof(Node *));

    int paths_size = 0;
    int paths_capacity = 10;
    Path *paths = calloc(paths_capacity, sizeof(Path));

    if (setjmp(exit_buf) != 0) {
        free(paths);

        for (int i = 0; i < network.size; i++) {
            free(network.nodes[i]->name);
            free(network.nodes[i]);
        }

        free(network.nodes);
        free(input);
        return 0;
    }

    for (int i = map_start; i < fsize; i += 17) {
        parse_node_name(input + i, &network);
    }

    for (int i = map_start; i < fsize; i += 17) {
        Node *node = network.nodes[(i - (map_start)) / 17];

        if (node->name[2] == 'A') {
            Path path;
            path.steps = 0;
            path.current = node;

            if (paths_size == paths_capacity) {
                paths_capacity *= 2;
                Path *tmp_paths = realloc(paths, paths_capacity * sizeof(Path));

                if (tmp_paths == NULL) {
                    longjmp(exit_buf, 1);
                } else {
                    paths = tmp_paths;
                }
            }

            paths[paths_size] = path;
            paths_size += 1;
        }

        parse_node_data(input + i, node, &network);
    }

    for (int i = 0; i < paths_size; i++) {
        char *instruction = input;
        Path *path = paths + i;

        while (path->current->name[2] != 'Z') {
            if (*instruction == 'L') {
                path->current = path->current->left;
            } else if (*instruction == 'R') {
                path->current = path->current->right;
            }

            if (instruction - input == instructions_size - 1) {
                instruction = input;
            } else {
                instruction += 1;
            }

            path->steps += 1;
        }
    }

    ll result = paths[0].steps;

    for (int i = 1; i < paths_size; i++) {
        Path *path = paths + i;
        result = ((ll)path->steps * result) / gcd((ll)path->steps, result);
    }

    printf("%lld\n", result);
    longjmp(exit_buf, 0);
}
