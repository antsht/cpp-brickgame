#include "brick_game/snake/snake_controller.h"
#include "gui/console/console_view.h"

int main(int argc [[maybe_unused]], char *argv[]) {
  s21::SnakeModel model(std::string(dirname(argv[0])));
  s21::SnakeController controller(&model);
  s21::BrickGameConsoleView view(&controller);
  view.StartEventLoop();
  return 1;
}