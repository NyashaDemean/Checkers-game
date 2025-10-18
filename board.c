#include <stdio.h>
#include "board.h"

// Turn ON a bit
void set_bit(uint64_t *board, int position) {
    *board |= (1ULL << position);
}

// Turn OFF a bit
void clear_bit(uint64_t *board, int position) {
    *board &= ~(1ULL << position);
}

// Check if a bit is ON
int check_bit(uint64_t board, int position) {
    return (board >> position) & 1ULL;
}

// Print 8x8 board with kings
void print_board(uint64_t red_pieces, uint64_t black_pieces, uint64_t red_kings, uint64_t black_kings) {
    printf("\n   ==== CHECKERS BOARD ====\n");
    for (int row = 7; row >= 0; row--) {
        printf("%d | ", row + 1);
        for (int col = 0; col < 8; col++) {
            int pos = row * 8 + col;
            if (check_bit(red_kings, pos)) printf("K ");
            else if (check_bit(black_kings, pos)) printf("k ");
            else if (check_bit(red_pieces, pos)) printf("R ");
            else if (check_bit(black_pieces, pos)) printf("B ");
            else printf(". ");
        }
        printf("\n");
    }
    printf("    a b c d e f g h\n");
}

