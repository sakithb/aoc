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

    int current_nodes_size = 0;
    int current_nodes_capacity = 10;
    Node **current_nodes = calloc(current_nodes_size, sizeof(Node *));

    if (setjmp(exit_buf) != 0) {
        free(current_nodes);

        for (int i = 0; i < network.size; i++) {
            free(network.nodes[i]->name);
            free(network.nodes[i]);
        }

        free(network.nodes);
        free(input);
        return 0;
    }

    int steps = 0;
    char *instruction = input;

    for (int i = map_start; i < fsize; i += 17) {
        parse_node_name(input + i, &network);
    }

    for (int i = map_start; i < fsize; i += 17) {
        Node *node = network.nodes[(i - (map_start)) / 17];

        if (node->name[2] == 'A') {
            if (current_nodes_size == current_nodes_capacity) {
                current_nodes_capacity *= 2;
                Node **tmp_nodes = realloc(current_nodes, current_nodes_capacity * sizeof(Node *));

                if (tmp_nodes == NULL) {
                    longjmp(exit_buf, 1);
                } else {
                    current_nodes = tmp_nodes;
                }
            }

            current_nodes[current_nodes_size] = node;
            current_nodes_size += 1;
        }

        parse_node_data(input + i, node, &network);
    }

    int completed_current_nodes = 0;

    while (completed_current_nodes < current_nodes_size) {
        completed_current_nodes = 0;

        for (int i = 0; i < current_nodes_size; i++) {
            Node *current = current_nodes[i];

            if (*instruction == 'L') {
                current = current->left;
            } else if (*instruction == 'R') {
                current = current->right;
            }

            if (current->name[2] == 'Z') {
                completed_current_nodes += 1;
            }

            current_nodes[i] = current;
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
