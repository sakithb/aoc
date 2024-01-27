#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;

typedef struct Node Node;
typedef struct Network Network;

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

    if (setjmp(exit_buf) != 0) {
        for (int i = 0; i < network.size; i++) {
            free(network.nodes[i]->name);
            free(network.nodes[i]);
        }

        free(network.nodes);
        free(input);
        return 0;
    }

    int steps = 0;
    Node *current = NULL;
    char *instruction = input;

    for (int i = map_start; i < fsize; i += 17) {
        parse_node_name(input + i, &network);
    }

    for (int i = map_start; i < fsize; i += 17) {
        Node *node = network.nodes[(i - (map_start)) / 17];

        if (strcmp(node->name, "AAA") == 0) {
            current = node;
        }

        parse_node_data(input + i, node, &network);
    }

    if (current == NULL) {
        longjmp(exit_buf, 1);
    }

    while (strcmp(current->name, "ZZZ") != 0) {
        if (*instruction == 'L') {
            current = current->left;
        } else if (*instruction == 'R') {
            current = current->right;
        }

        if (instruction - input == instructions_size - 1) {
            instruction = input;
        } else {
            instruction += 1;
        }

        steps += 1;
    }

    printf("%d\n", steps);
    longjmp(exit_buf, 0);
}
