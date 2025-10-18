#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Bit manipulation helpers
void set_bit(uint64_t *board, int position);
void clear_bit(uint64_t *board, int position);
int check_bit(uint64_t board, int position);

// Display helpers
void print_board(uint64_t red_pieces, uint64_t black_pieces, uint64_t red_kings, uint64_t black_kings);


#endif
