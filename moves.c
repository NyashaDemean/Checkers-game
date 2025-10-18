#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"
#include "moves.h"

// Convert coordinate like "a1" -> 0–63
int coord_to_index(char file, int rank) {
    int col = file - 'a';
    int row = rank - 1;
    return row * 8 + col;
}

// Check if a move is valid (normal + king)
int is_valid_move(uint64_t board, uint64_t opponent, int from, int to, int is_red, uint64_t kings) {
    if (!check_bit(board, from) && !check_bit(kings, from)) {
        printf("❌ No piece at that position.\n");
        return 0;
    }
    if (check_bit(board, to) || check_bit(opponent, to)) {
        printf("❌ Destination square occupied.\n");
        return 0;
    }

    int row_from = from / 8, col_from = from % 8;
    int row_to = to / 8, col_to = to % 8;
    if (abs(row_from - row_to) != 1 || abs(col_from - col_to) != 1) {
        printf("❌ Invalid direction: must move diagonally by 1.\n");
        return 0;
    }

    // Red moves up, black down — unless it’s a king
    int is_king = check_bit(kings, from);
    if (!is_king) {
        if (is_red && row_to <= row_from) {
            printf("❌ Red can only move forward.\n");
            return 0;
        }
        if (!is_red && row_to >= row_from) {
            printf("❌ Black can only move forward.\n");
            return 0;
        }
    }

    return 1;
}

// Try to capture (normal or king)
int try_capture(uint64_t *player_board, uint64_t *opponent_board, int from, int to, int is_red, uint64_t *player_kings) {
    int row_from = from / 8, col_from = from % 8;
    int row_to = to / 8, col_to = to % 8;
    int is_king = check_bit(*player_kings, from);

    if (abs(row_from - row_to) != 2 || abs(col_from - col_to) != 2) {
        return 0;
    }

    int row_mid = (row_from + row_to) / 2;
    int col_mid = (col_from + col_to) / 2;
    int mid = row_mid * 8 + col_mid;

    if (!is_king) {
        if (is_red && row_to <= row_from) return 0;
        if (!is_red && row_to >= row_from) return 0;
    }

    if (!check_bit(*opponent_board, mid)) return 0;
    if (check_bit(*player_board, to) || check_bit(*opponent_board, to)) return 0;

    // ✅ Valid capture
clear_bit(opponent_board, mid);
clear_bit(player_board, from);
clear_bit(player_kings, from);
set_bit(player_board, to);
check_kinging(player_board, player_kings, is_red);

printf("✅ Capture successful! Opponent piece removed.\n");

// Check for another jump
if (can_capture_again(*player_board, *opponent_board, to, is_red, *player_kings)) {
    printf("🔁 Another capture available from this piece!\n");
}
return 1;

}

// Check if a piece can capture again after a jump
int can_capture_again(uint64_t player_board, uint64_t opponent_board, int from, int is_red, uint64_t player_kings) {
    int row = from / 8;
    int col = from % 8;
    int directions[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};  // all diagonals

    int is_king = check_bit(player_kings, from);

    for (int i = 0; i < 4; i++) {
        int dr = directions[i][0];
        int dc = directions[i][1];

        // Restrict direction for non-kings
        if (!is_king) {
            if (is_red && dr < 0) continue;
            if (!is_red && dr > 0) continue;
        }

        int mid_row = row + dr;
        int mid_col = col + dc;
        int dest_row = row + 2*dr;
        int dest_col = col + 2*dc;

        if (mid_row < 0 || mid_row > 7 || mid_col < 0 || mid_col > 7 ||
            dest_row < 0 || dest_row > 7 || dest_col < 0 || dest_col > 7)
            continue;

        int mid = mid_row * 8 + mid_col;
        int dest = dest_row * 8 + dest_col;

        if (check_bit(opponent_board, mid) &&
            !check_bit(player_board, dest) &&
            !check_bit(opponent_board, dest)) {
            return 1;  // another capture available
        }
    }
    return 0;
}

// Count how many pieces a player has (normal + kings)
int count_pieces(uint64_t board) {
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (check_bit(board, i)) count++;
    }
    return count;
}

// Check if player has any possible move or capture
int has_any_moves(uint64_t player_board, uint64_t opponent_board, int is_red, uint64_t player_kings) {
    for (int from = 0; from < 64; from++) {
        if (!check_bit(player_board, from) && !check_bit(player_kings, from)) continue;

        int row = from / 8;
        int col = from % 8;
        int directions[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
        int is_king = check_bit(player_kings, from);

        for (int i = 0; i < 4; i++) {
            int dr = directions[i][0];
            int dc = directions[i][1];

            if (!is_king) {
                if (is_red && dr < 0) continue;
                if (!is_red && dr > 0) continue;
            }

            // Normal move
            int row_to = row + dr;
            int col_to = col + dc;
            if (row_to >= 0 && row_to < 8 && col_to >= 0 && col_to < 8) {
                int to = row_to * 8 + col_to;
                if (!check_bit(player_board, to) && !check_bit(opponent_board, to)) {
                    return 1; // has at least one move
                }
            }

            // Capture move
            int mid_row = row + dr;
            int mid_col = col + dc;
            int dest_row = row + 2 * dr;
            int dest_col = col + 2 * dc;
            if (dest_row >= 0 && dest_row < 8 && dest_col >= 0 && dest_col < 8) {
                int mid = mid_row * 8 + mid_col;
                int dest = dest_row * 8 + dest_col;
                if (check_bit(opponent_board, mid) &&
                    !check_bit(player_board, dest) &&
                    !check_bit(opponent_board, dest)) {
                    return 1; // can capture
                }
            }
        }
    }
    return 0; // no moves available
}



// Regular move (non-capture)
void move_piece(uint64_t *board, int from, int to) {
    clear_bit(board, from);
    set_bit(board, to);
}

// Check and crown kings
void check_kinging(uint64_t *player_board, uint64_t *player_kings, int is_red) {
    for (int col = 0; col < 8; col++) {
        int top = 56 + col; // top row (8th)
        int bottom = col;   // bottom row (1st)

        if (is_red && check_bit(*player_board, top))
            set_bit(player_kings, top);
        if (!is_red && check_bit(*player_board, bottom))
            set_bit(player_kings, bottom);
    }
}
