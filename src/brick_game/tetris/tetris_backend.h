#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "../common.h"
#include "defines_tetris.h"
#include "fsm.h"
#include "objects.h"

/**
 * Initializes the game statistics structure with default values.
 *
 * @param stats Pointer to the game_stats_t structure to be initialized.
 */
void init_stats(game_stats_t *stats);
/**
 * Initializes the game board with default values.
 *
 * @param board Pointer to the board_t structure to be initialized.
 */
void init_board(board_t *board);

/**
 * Generates the next tetramino to be used in the game.
 *
 * @return The newly generated tetramino.
 */
tetramino_t gen_next_tetramino();

/**
 * Checks if the given tetramino collides with the left border of the game
 * board.
 *
 * @param tetramino Pointer to the tetramino to check for collision.
 * @param board Pointer to the game board.
 * @return true if the tetramino collides with the left border, false otherwise.
 */
bool check_lborder_collide(const tetramino_t *tetramino);
/**
 * Checks if the given tetramino collides with the right border of the game
 * board.
 *
 * @param tetramino Pointer to the tetramino to check for collision.
 * @return true if the tetramino collides with the right border, false
 * otherwise.
 */
bool check_rborder_collide(const tetramino_t *tetramino);
/**
 * Checks if the given tetramino collides with the game board.
 *
 * @param tetramino Pointer to the tetramino to check for collision.
 * @return true if the tetramino collides with the board, false otherwise.
 */
bool check_board_collide(const tetramino_t *tetramino, const board_t *board);
/**
 * Attaches the current tetramino to the game board.
 *
 * @param board Pointer to the game board to attach the tetramino to.
 */
void attach_tetramino(board_t *board);

/**
 * Finds the full rows in the game board and returns the number of full rows.
 *
 * @param board Pointer to the game board.
 * @return The number of full rows in the game board.
 */
int find_full_rows(const board_t *board);
/**
 * Shifts the game board by the specified number of rows.
 *
 * @param board Pointer to the game board to be shifted.
 * @param row_number The number of rows to shift the board.
 */
void shift_board(board_t *board, int row_number);
/**
 * Updates the game score based on the number of rows removed.
 *
 * @param stats Pointer to the game statistics struct to update.
 * @param rows_removed The number of rows removed in the current game tick.
 */
void update_score(game_stats_t *stats, int rows_removed);

/**
 * Saves the high score to persistent storage.
 *
 * @param stats Pointer to the game statistics struct containing the high score
 * to save.
 * @return 0 on success, non-zero on failure.
 */
int save_high_score(const game_stats_t *stats);
/**
 * Loads the high score from persistent storage.
 *
 * @param stats Pointer to the game statistics struct to load the high score
 * into.
 * @return 0 on success, non-zero on failure.
 */
int load_high_score(game_stats_t *stats);
/**
 * Returns the current time in milliseconds.
 *
 * @return The current time in milliseconds.
 */
uint64_t timeInMilliseconds(void);

#endif