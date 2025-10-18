#include "save_load.h"
#include <stdio.h>
#include <stdint.h>

// Save current game state to file
void save_game(const char *filename, uint64_t red_pieces, uint64_t black_pieces, uint64_t red_kings, uint64_t black_kings, int turn, int idle_turns) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("❌ Error: Could not save game to %s\n", filename);
        return;
    }

    fwrite(&red_pieces, sizeof(uint64_t), 1, file);
    fwrite(&black_pieces, sizeof(uint64_t), 1, file);
    fwrite(&red_kings, sizeof(uint64_t), 1, file);
    fwrite(&black_kings, sizeof(uint64_t), 1, file);
    fwrite(&turn, sizeof(int), 1, file);
    fwrite(&idle_turns, sizeof(int), 1, file);

    fclose(file);
    printf("💾 Game saved successfully to '%s'!\n", filename);
}

// Load game state from file
int load_game(const char *filename, uint64_t *red_pieces, uint64_t *black_pieces, uint64_t *red_kings, uint64_t *black_kings, int *turn, int *idle_turns) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("❌ Error: Could not load game from %s\n", filename);
        return 0;
    }

    fread(red_pieces, sizeof(uint64_t), 1, file);
    fread(black_pieces, sizeof(uint64_t), 1, file);
    fread(red_kings, sizeof(uint64_t), 1, file);
    fread(black_kings, sizeof(uint64_t), 1, file);
    fread(turn, sizeof(int), 1, file);
    fread(idle_turns, sizeof(int), 1, file);

    fclose(file);
    printf("📂 Game loaded successfully from '%s'!\n", filename);
    return 1;
}
