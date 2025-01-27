#ifndef FSM_H
#define FSM_H

#include "defines_tetris.h"
#include "objects.h"
#include "tetris_backend.h"

/**
 * Represents the different states of the game state machine.
 *
 * - START: The initial state of the game.
 * - SPAWN: The state when a new game piece is spawned.
 * - MOVING: The state when the game piece is actively moving.
 * - MOVE: The state when the game piece is moved.
 * - ATTACHING: The state when the game piece is attaching to the board.
 * - GAMEOVER: The state when the game is over.
 * - PAUSE: The state when the game is paused.
 * - EXIT_STATE: The state when the game is exiting.
 */
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  MOVE,
  ATTACHING,
  GAMEOVER,
  PAUSE,
  EXIT_STATE
} game_state;

/**
 * Represents the different signals that can be received by the game state
 * machine.
 *
 * - ACTION_BTN: The spacebar button is pressed.
 * - MOVE_DOWN: The down arrow key is pressed.
 * - MOVE_RIGHT: The right arrow key is pressed.
 * - MOVE_LEFT: The left arrow key is pressed.
 * - PAUSE_BTN: The pause button is pressed.
 * - START_BTN: The start button is pressed.
 * - ESCAPE_BTN: The escape button is pressed, indicating the game should end.
 * - NOSIG: No signal is received.
 */
typedef enum {
  ACTION_BTN = 0,  // spacebar
  MOVE_DOWN,
  MOVE_RIGHT,
  MOVE_LEFT,
  PAUSE_BTN,
  START_BTN,
  ESCAPE_BTN,  // end game
  NOSIG
} signals;

/**
 * Rotates the current game piece clockwise.
 *
 * @param state The current game state.
 * @param board The current game board.
 */
void rotate(game_state *state, board_t *board);

/**
 * Moves the current game piece down on the board.
 *
 * @param state The current game state.
 * @param board The current game board.
 */
void movedown(game_state *state, board_t *board);

/**
 * Moves the current game piece to the right on the board.
 *
 * @param board The current game board.
 */
void moveright(board_t *board);

/**
 * Moves the current game piece to the left on the board.
 *
 * @param board The current game board.
 */
void moveleft(board_t *board);

/**
 * Handles the start state of the game state machine.
 *
 * @param sig The signal that triggered the state transition.
 * @param state The current game state.
 */
void on_start_state(signals sig, game_state *state, game_stats_t *stats);

/**
 * Handles the spawn state of the game state machine.
 *
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void on_spawn_state(game_state *state, board_t *board);

/**
 * Handles the pause state of the game state machine.
 *
 * @param sig The signal that triggered the state transition.
 * @param state The current game state.
 */
void on_pause_state(signals sig, game_state *state);

/**
 * Handles the moving state of the game state machine.
 *
 * @param sig The signal that triggered the state transition.
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void on_moving_state(signals sig, game_state *state, board_t *board);

/**
 * Handles the attach state of the game state machine.
 *
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void on_attach_state(game_state *state, game_stats_t *stats, board_t *board);

/**
 * Handles the move state of the game state machine.
 *
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void on_move_state(game_state *state, game_stats_t *stats, board_t *board);

/**
 * Handles the game over state of the game state machine.
 *
 * @param sig The signal that triggered the state transition.
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void on_gameover_state(signals sig, game_state *state, game_stats_t *stats,
                       board_t *board);

/**
 * Retrieves the signal corresponding to the given user input.
 *
 * @param user_input The user input to convert to a signal.
 * @return The signal corresponding to the given user input.
 */
signals getSignal(int user_input);

/**
 * Handles the state transitions for the game state machine.
 *
 * @param sig The signal that triggered the state transition.
 * @param state The current game state.
 * @param stats The current game statistics.
 * @param board The current game board.
 */
void sigact(signals sig, game_state *state, game_stats_t *stats,
            board_t *board);

#endif