#include "brick_game/tetris/tetris_controller.h"
#include "brick_game/tetris/tetris_game_info_t_raii.h"
#include "gui/desktop/GUI_view.h"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("s21.school.robynarl.brickgame_2_0");

  GameInfo game_info;

  board_t board = {
      .board = {},           // Zero initialize board
      .tetramino_curr = {},  // Zero initialize tetramino_curr
      .tetramino_next = {}   // Zero initialize tetramino_next
  };
  init_board(&board);

  game_stats_t stats;
  init_stats(&stats);

  game_state state = START;

  s21::TetrisController controller(game_info.get(), &state, &board, &stats);

  app->make_window_and_run<s21::GUIView>(argc, argv, &controller);

  return 0;
}
