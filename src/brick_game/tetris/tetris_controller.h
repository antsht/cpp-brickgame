#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H

#include <stdexcept>

#include "../common.h"
#include "../controller.h"
#include "fsm.h"
#include "tetris_backend.h"

namespace s21 {

class TetrisController : public Controller {
 private:
 public:
  GameInfo_t* game_info;
  game_state* state;
  board_t* board;
  game_stats_t* stats;
  TetrisController(GameInfo_t* game_info, game_state* state, board_t* board,
                   game_stats_t* stats);
  void UpdateCurrentState() override;
  void processUserInput(UserAction_t action, bool hold) override;
};
}  // namespace s21

#endif  // TETRIS_CONTROLLER_H