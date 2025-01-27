#include "tetris_controller.h"

#include <chrono>

namespace s21 {

Controller* Controller::instance = nullptr;  // Define the static member

TetrisController::TetrisController(GameInfo_t* game_info, game_state* state,
                                   board_t* board, game_stats_t* stats)
    : game_info(game_info), state(state), board(board), stats(stats) {
  if (instance != nullptr) {
    throw std::runtime_error("Controller instance already exists.");
  }
  instance = this;
};

void TetrisController::UpdateCurrentState() {
  *game_info = ::updateCurrentState();
}

void TetrisController::processUserInput(UserAction_t action,
                                        [[maybe_unused]] bool hold) {
  ::userInput(action, hold);
}

std::chrono::_V2::system_clock::time_point frame_start_in_ms =
    std::chrono::system_clock::now();
}  // namespace s21

GameInfo_t updateCurrentState() {
  GameInfo_t* game_info =
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))
          ->game_info;
  const game_state* game_state =
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->state;
  const game_stats_t* game_stats =
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->stats;
  board_t* game_board =
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->board;

  auto current_time_in_ms = std::chrono::system_clock::now();
  auto elapsed_time_in_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          current_time_in_ms - s21::frame_start_in_ms)
          .count();
  if (elapsed_time_in_ms > (500 - 35 * game_stats->level)) {
    s21::frame_start_in_ms = current_time_in_ms;
    userInput(UserAction_t::Down, false);
  }

  game_info->score = game_stats->score;
  game_info->level = game_stats->level;
  game_info->high_score = game_stats->high_score;
  game_info->pause = (*game_state == PAUSE);
  if (*game_state == GAMEOVER) {
    game_info->level = kLoose;
  }

  // deep copy of the board
  for (int i = 0; i < kFieldHeight; i++) {
    for (int j = 0; j < kFieldWidth; j++) {
      game_info->field[i][j] = game_board->board[i][j];
    }
  }

  // deep copy of current tetramino
  for (int i = 0; i < kNextFieldHeight; i++) {
    for (int j = 0; j < kNextFieldWidth; j++) {
      if (game_board->tetramino_curr.figure
              .sprite[game_board->tetramino_curr.rotation][i][j] != 0) {
        game_info->field[game_board->tetramino_curr.row_pos + i]
                        [game_board->tetramino_curr.col_pos + j] =
            game_board->tetramino_curr.figure.figure_color;
      }
    }
  }

  // deep copy of next tetramino
  for (int i = 0; i < kNextFieldHeight; i++) {
    for (int j = 0; j < kNextFieldWidth; j++) {
      if (game_board->tetramino_next.figure
              .sprite[game_board->tetramino_next.rotation][i][j] != 0) {
        game_info->next[i][j] = game_board->tetramino_next.figure.figure_color;
      } else {
        game_info->next[i][j] = 0;
      }
    }
  }

  return *game_info;
}

void userInput(UserAction_t action, [[maybe_unused]] bool hold) {
  signals sig;
  switch (action) {
    case UserAction_t::Start:
      sig = START_BTN;
      break;
    case UserAction_t::Pause:
      sig = PAUSE_BTN;
      break;
    case UserAction_t::Action:
      sig = ACTION_BTN;
      break;
    case UserAction_t::Terminate:
      sig = ESCAPE_BTN;
      break;
    case UserAction_t::Left:
      sig = MOVE_LEFT;
      break;
    case UserAction_t::Right:
      sig = MOVE_RIGHT;
      break;
    case UserAction_t::Up:
      sig = NOSIG;
      break;
    case UserAction_t::Down:
      sig = MOVE_DOWN;
      break;
    default:
      sig = NOSIG;
      break;
  }
  sigact(
      sig,
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->state,
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->stats,
      (dynamic_cast<s21::TetrisController*>(s21::Controller::instance))->board);

  return;
}
