#include "brick_game/snake/snake_controller.h"
#include "gui/desktop/GUI_view.h"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("s21.school.robynarl.brickgame_2_0");
  s21::SnakeModel model(std::string(dirname(argv[0])));
  s21::SnakeController controller(&model);
  return app->make_window_and_run<s21::GUIView>(argc, argv, &controller);
}
