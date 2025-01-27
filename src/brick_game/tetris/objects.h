#ifndef OBJECTS_H
#define OBJECTS_H
#include <cstdint>

#include "defines_tetris.h"

/**
 * Represents a tetris figure, including its sprite and maximum number of
 * rotations.
 */
typedef struct {
  int sprite[4][4][4];
  int max_rotation;
  int figure_color;
} figure_t;

/**
 * Represents a tetromino, which is a Tetris piece. It includes the current row
 * and column position, the figure (sprite) of the tetromino, and the current
 * rotation.
 */
typedef struct {
  int row_pos;
  int col_pos;
  figure_t figure;
  int rotation;
} tetramino_t;

/**
 * Represents the game board, including the current and next tetramino, and the
 * state of the board.
 */
typedef struct {
  int board[BOARD_ROWS][BOARD_COLS];
  tetramino_t tetramino_curr;
  tetramino_t tetramino_next;
} board_t;

/**
 * Represents the current game statistics, including the player's score, high
 * score, current level, and the time until the next level advancement.
 */
typedef struct {
  int score;
  int high_score;
  int level;
  uint64_t next_advance_time_in_ms;
} game_stats_t;

#endif