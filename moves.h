#ifndef MOVES_H
#define MOVES_H

#include <stdint.h>

//moving pieces 
int coord_to_index(char file, int rank);
int is_valid_move(uint64_t board, uint64_t opponent, int from, int to, int is_red, uint64_t kings);
void move_piece(uint64_t *board, int from, int to);
int try_capture(uint64_t *player_board, uint64_t *opponent_board, int from, int to, int is_red, uint64_t *player_kings);
void check_kinging(uint64_t *player_board, uint64_t *player_kings, int is_red);
int can_capture_again(uint64_t player_board, uint64_t opponent_board, int from, int is_red, uint64_t player_kings);
int has_any_moves(uint64_t player_board, uint64_t opponent_board, int is_red, uint64_t player_kings);
int count_pieces(uint64_t board);


#endif

