#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stdint.h>

// Save and load functions
void save_game(const char *filename, uint64_t red_pieces, uint64_t black_pieces, uint64_t red_kings, uint64_t black_kings, int turn, int idle_turns);
int load_game(const char *filename, uint64_t *red_pieces, uint64_t *black_pieces, uint64_t *red_kings, uint64_t *black_kings, int *turn, int *idle_turns);

#endif
