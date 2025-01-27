#include "fsm.h"

#include <time.h>

void rotate(game_state *state, board_t *board) {
  int curr_rotation = board->tetramino_curr.rotation;
  board->tetramino_curr.rotation =
      (board->tetramino_curr.rotation + 1) %
      (board->tetramino_curr.figure.max_rotation + 1);
  if (check_board_collide(&(board->tetramino_curr), board)) {
    board->tetramino_curr.rotation = curr_rotation;
    *state = ATTACHING;
  } else {
    while (check_lborder_collide(&(board->tetramino_curr)))
      board->tetramino_curr.col_pos++;
    while (check_rborder_collide(&(board->tetramino_curr)))
      board->tetramino_curr.col_pos--;
  }
}

void movedown(game_state *state, board_t *board) {
  tetramino_t tmp = board->tetramino_curr;
  tmp.row_pos++;
  if (!check_board_collide(&tmp, board)) {
    board->tetramino_curr.row_pos++;
    *state = MOVING;
  } else {
    *state = ATTACHING;
  }
}

void moveright(board_t *board) {
  tetramino_t tmp = board->tetramino_curr;
  tmp.col_pos++;
  if (!check_rborder_collide(&tmp) && !check_board_collide(&tmp, board)) {
    board->tetramino_curr.col_pos++;
  }
}

void moveleft(board_t *board) {
  tetramino_t tmp = board->tetramino_curr;
  tmp.col_pos--;
  if (!check_lborder_collide(&tmp) && !check_board_collide(&tmp, board)) {
    board->tetramino_curr.col_pos--;
  }
}

void on_start_state(signals sig, game_state *state, game_stats_t *stats) {
  switch (sig) {
    case START_BTN:
      *state = SPAWN;
      stats->level = kLevel1;
      break;
    case ESCAPE_BTN:
      *state = EXIT_STATE;
      break;
    default:
      *state = START;
      break;
  }
}

void on_spawn_state(game_state *state, board_t *board) {
  *state = MOVING;

  board->tetramino_curr = board->tetramino_next;
  board->tetramino_next = gen_next_tetramino();

  if (check_board_collide(&(board->tetramino_curr), board)) {
    *state = GAMEOVER;
  }
}

void on_pause_state(signals sig, game_state *state) {
  // print_pause_banner();
  if (sig == PAUSE_BTN) {
    *state = MOVING;
  }
}

void on_moving_state(signals sig, game_state *state, board_t *board) {
  switch (sig) {
    case ACTION_BTN:
      rotate(state, board);
      break;
    case MOVE_DOWN:
      movedown(state, board);
      break;
    case MOVE_RIGHT:
      moveright(board);
      break;
    case MOVE_LEFT:
      moveleft(board);
      break;
    case PAUSE_BTN:
      *state = PAUSE;
      break;
    case ESCAPE_BTN:
      *state = EXIT_STATE;
      break;
    default:
      break;
  }
}

void on_attach_state(game_state *state, game_stats_t *stats, board_t *board) {
  attach_tetramino(board);
  int rows_removed = 0;
  int full_row_number = 0;
  while ((full_row_number = find_full_rows(board)) && full_row_number != -1) {
    shift_board(board, full_row_number);
    rows_removed++;
  }
  update_score(stats, rows_removed);

  *state = SPAWN;
}

void on_move_state(game_state *state, game_stats_t *stats, board_t *board) {
  stats->next_advance_time_in_ms =
      timeInMilliseconds() + 500 - 50 * (stats->level - 1);
  movedown(state, board);
}

void on_gameover_state(signals sig, game_state *state, game_stats_t *stats,
                       board_t *board) {
  stats->level = kLoose;
  if (sig == START_BTN) {
    init_board(board);
    init_stats(stats);
    stats->level = kLevel1;
    *state = SPAWN;
  } else if (sig == ESCAPE_BTN)
    *state = EXIT_STATE;
}

// cppcheck-suppress unusedFunction
void sigact(signals sig, game_state *state, game_stats_t *stats,
            board_t *board) {
  switch (*state) {
    case START:
      on_start_state(sig, state, stats);
      break;
    case SPAWN:
      on_spawn_state(state, board);
      break;
    case MOVING:
      on_moving_state(sig, state, board);
      break;
    case MOVE:
      on_move_state(state, stats, board);
      break;
    case ATTACHING:
      on_attach_state(state, stats, board);
      break;
    case PAUSE:
      on_pause_state(sig, state);
      break;
    case GAMEOVER:
      on_gameover_state(sig, state, stats, board);
      break;
    default:
      break;
  }
}