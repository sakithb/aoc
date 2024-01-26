#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf exit_buf;
static char labels[] = "AKQJT98765432";

typedef struct {
    int bid;
    char cards[6];
} Hand;

typedef struct {
    Hand *arr;
    int size;
    int capacity;
} Hands;

typedef enum { FIVE_OF_A_KIND, FOUR_OF_A_KIND, FULL_HOUSE, THREE_OF_A_KIND, TWO_PAIRS, ONE_PAIR, HIGH_CARD } HandType;

void parse_hand(char *line, Hands *hands) {
    char cards[6];
    int bid;

    sscanf(line, "%s %d\n", cards, &bid);

    Hand hand;
    hand.bid = bid;
    strcpy(hand.cards, cards);

    if (hands->size == hands->capacity) {
        hands->capacity *= 2;
        Hand *tmp_hands = realloc(hands->arr, hands->capacity * sizeof(Hand));

        if (tmp_hands == NULL) {
            longjmp(exit_buf, 1);
        } else {
            hands->arr = tmp_hands;
        }
    }

    hands->arr[hands->size] = hand;
    hands->size += 1;
}

HandType get_hand_type(Hand *hand) {
    static int counts[13];
    memset(counts, 0, sizeof(counts));

    int dup_groups = 0;
    int dup_singles = 0;

    for (int i = 0; i < 5; i++) {
        int pos = strchr(labels, hand->cards[i]) - labels;
        counts[pos] += 1;

        if (counts[pos] == 2) {
            dup_groups++;
            dup_singles += 2;
        } else if (counts[pos] > 2) {
            dup_singles++;
        }
    }

    if (dup_groups == 1 && dup_singles == 5) {
        return FIVE_OF_A_KIND;
    } else if (dup_groups == 1 && dup_singles == 4) {
        return FOUR_OF_A_KIND;
    } else if (dup_groups == 2 && dup_singles == 5) {
        return FULL_HOUSE;
    } else if (dup_groups == 1 && dup_singles == 3) {
        return THREE_OF_A_KIND;
    } else if (dup_groups == 2 && dup_singles == 4) {
        return TWO_PAIRS;
    } else if (dup_groups == 1 && dup_singles == 2) {
        return ONE_PAIR;
    } else {
        return HIGH_CARD;
    }
}

int cmp_hands(const void *a, const void *b) {
    Hand *hand_a = (Hand *)a;
    Hand *hand_b = (Hand *)b;

    HandType hand_type_a = get_hand_type(hand_a);
    HandType hand_type_b = get_hand_type(hand_b);

    if (hand_type_a == hand_type_b) {
        for (int i = 0; i < 5; i++) {
            int a_pos = strchr(labels, hand_a->cards[i]) - labels;
            int b_pos = strchr(labels, hand_b->cards[i]) - labels;

            if (a_pos == b_pos) {
                continue;
            }

            return b_pos - a_pos;
        }

        return 0;
    }

    return hand_type_b - hand_type_a;
}

int main() {
    FILE *file = fopen("input.txt", "r");

    Hands hands;
    hands.size = 0;
    hands.capacity = 10;
    hands.arr = calloc(hands.capacity, sizeof(Hand));

    char line[12];

    if (setjmp(exit_buf) != 0) {
        free(hands.arr);
        fclose(file);
        return 0;
    }

    while (fgets(line, 12, file) != NULL) {
        parse_hand(line, &hands);
    }

    qsort(hands.arr, hands.size, sizeof(Hand), cmp_hands);
    int winnings = 0;

    for (int i = 0; i < hands.size; i++) {
        winnings += hands.arr[i].bid * (i + 1);
    }

    printf("%d\n", winnings);
    longjmp(exit_buf, 0);
}
