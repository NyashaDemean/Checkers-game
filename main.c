#include "board.h"
#include "moves.h"
#include "save_load.h"
#include <stdio.h>
#include <stdint.h>

int main() {
    uint64_t red_pieces = 0, black_pieces = 0;
    uint64_t red_kings = 0, black_kings = 0;
    int turn = 0; // 0 = Red, 1 = Black
    int game_over = 0;
    int idle_turns = 0;

    printf("=== CHECKERS GAME ===\n");
    printf("Enter moves like \"b2 c3\" or type \"q\" to quit.\n");
    printf("Commands: s = save, l = load\n\n");

    // ---- Starting setup ----
    set_bit(&red_pieces, 9);    // b2
    set_bit(&black_pieces, 18); // c3
    set_bit(&black_pieces, 35); // d4
    // ------------------------

    while (!game_over) {
        print_board(red_pieces, black_pieces, red_kings, black_kings);
        printf("\n%s's turn.\n", turn == 0 ? "Red" : "Black");
        printf("Enter move (e.g., b2 c3), or (s)ave / (l)oad / (q)uit: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        if (input[0] == 'q') {
            printf("👋 Exiting game...\n");
            break;
            }
        else if (input[0] == 's') {
            save_game("checkers_save.dat", red_pieces, black_pieces, red_kings, black_kings, turn, idle_turns);
            continue;
        }
        else if (input[0] == 'l') {
            if (load_game("checkers_save.dat", &red_pieces, &black_pieces, &red_kings, &black_kings, &turn, &idle_turns))
            print_board(red_pieces, black_pieces, red_kings, black_kings);
        continue;
        }


        char from_file, to_file;
        int from_rank, to_rank;

        if (sscanf(input, " %c%d %c%d", &from_file, &from_rank, &to_file, &to_rank) == 4) {
            int from = coord_to_index(from_file, from_rank);
            int to = coord_to_index(to_file, to_rank);

            if (turn == 0) { // RED
                if (try_capture(&red_pieces, &black_pieces, from, to, 1, &red_kings)) {
                    move_piece(&red_pieces, from, to);
                    check_kinging(&red_pieces, &red_kings, 1);
                    while (can_capture_again(red_pieces, black_pieces, to, 1, red_kings)) {
                        print_board(red_pieces, black_pieces, red_kings, black_kings);
                        printf("🔁 Red can capture again! Enter next jump (e.g., d4 f6): ");
                        char next[10];
                        fgets(next, sizeof(next), stdin);
                        char nf, tf;
                        int nr, tr;
                        if (sscanf(next, " %c%d %c%d", &nf, &nr, &tf, &tr) == 4) {
                            int nf_idx = coord_to_index(nf, nr);
                            int tf_idx = coord_to_index(tf, tr);
                            if (!try_capture(&red_pieces, &black_pieces, nf_idx, tf_idx, 1, &red_kings))
                                break;
                            move_piece(&red_pieces, nf_idx, tf_idx);
                            check_kinging(&red_pieces, &red_kings, 1);
                            to = tf_idx;
                        } else break;
                    }
                    turn = 1;
                } 
                else if (is_valid_move(red_pieces, black_pieces, from, to, 1, red_kings)) {
                    move_piece(&red_pieces, from, to);
                    check_kinging(&red_pieces, &red_kings, 1);
                    turn = 1;
                } else {
                    printf("Invalid move.\n");
                }

            } else { // BLACK
                if (try_capture(&black_pieces, &red_pieces, from, to, 0, &black_kings)) {
                    move_piece(&black_pieces, from, to);
                    check_kinging(&black_pieces, &black_kings, 0);
                    while (can_capture_again(black_pieces, red_pieces, to, 0, black_kings)) {
                        print_board(red_pieces, black_pieces, red_kings, black_kings);
                        printf("🔁 Black can capture again! Enter next jump (e.g., f6 d4): ");
                        char next[10];
                        fgets(next, sizeof(next), stdin);
                        char nf, tf;
                        int nr, tr;
                        if (sscanf(next, " %c%d %c%d", &nf, &nr, &tf, &tr) == 4) {
                            int nf_idx = coord_to_index(nf, nr);
                            int tf_idx = coord_to_index(tf, tr);
                            if (!try_capture(&black_pieces, &red_pieces, nf_idx, tf_idx, 0, &black_kings))
                                break;
                            move_piece(&black_pieces, nf_idx, tf_idx);
                            check_kinging(&black_pieces, &black_kings, 0);
                            to = tf_idx;
                        } else break;
                    }
                    turn = 0;
                } 
                else if (is_valid_move(black_pieces, red_pieces, from, to, 0, black_kings)) {
                    move_piece(&black_pieces, from, to);
                    check_kinging(&black_pieces, &black_kings, 0);
                    turn = 0;
                } else {
                    printf("Invalid move.\n");
                }
            }

            // ✅ Check for game-over conditions after each move
            int red_count = count_pieces(red_pieces) + count_pieces(red_kings);
            int black_count = count_pieces(black_pieces) + count_pieces(black_kings);

            if (red_count == 0) {
                printf("\n🏁 Black wins! Red has no pieces left.\n");
                game_over = 1;
            } else if (black_count == 0) {
                printf("\n🏁 Red wins! Black has no pieces left.\n");
                game_over = 1;
            } else if (!has_any_moves(red_pieces, black_pieces, 1, red_kings) && turn == 0) {
                printf("\n🏁 Black wins! Red has no moves available.\n");
                game_over = 1;
            } else if (!has_any_moves(black_pieces, red_pieces, 0, black_kings) && turn == 1) {
                printf("\n🏁 Red wins! Black has no moves available.\n");
                game_over = 1;
            }

        } else {
            printf("Invalid input format.\n");
        }
    }

    printf("\nGame ended.\n");
    return 0;
}
