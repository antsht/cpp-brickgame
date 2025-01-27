#include "brick_game/tetris/tetris_backend.h"
#include "brick_game/tetris/tetris_controller.h"
#include "brick_game/tetris/tetris_game_info_t_raii.h"
#include "gui/console/console_view.h"

int main() {
  GameInfo game_info;

  board_t board = {.board = {}, .tetramino_curr = {}, .tetramino_next = {}};
  init_board(&board);

  game_stats_t stats;
  init_stats(&stats);

  game_state state = START;

  s21::TetrisController controller(game_info.get(), &state, &board, &stats);
  s21::BrickGameConsoleView view(&controller);
  view.StartEventLoop();

  return 0;
}