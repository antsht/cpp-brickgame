#include "GUI_view.h"

#include <iostream>

namespace s21 {

GUIView::GUIView(s21::Controller* c) : controller(c) {
  set_title("Brick Game");
  set_default_size(kWindowWidth, kWindowHeigth);

  controller->UpdateCurrentState();

  outer_box.set_orientation(Gtk::Orientation::VERTICAL);
  set_child(outer_box);

  // Top: Info Panel
  SetupTopStatusPanel();
  outer_box.append(top_status_panel);

  // Main horizontal box
  main_box.set_orientation(Gtk::Orientation::HORIZONTAL);

  // Left: Game Board
  SetupGameGrid();
  main_box.append(game_grid);

  // Right: Info Panel
  SetupInfoPanel();
  main_box.append(info_panel);

  outer_box.append(main_box);

  // Connect the signal handler for key press events
  auto input_controller = Gtk::EventControllerKey::create();
  input_controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &GUIView::OnWindowKeyPressed), false);
  add_controller(input_controller);

  // Start timer for updating the GUI
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &GUIView::OnTimeout),
                                 kDrawTimeoutInMS);
}

bool GUIView::OnTimeout() {
  controller->UpdateCurrentState();
  auto currentGameState = updateCurrentState();
  message_label->set_text(GetStatusMessage());
  game_canvas->queue_draw();
  next_canvas->queue_draw();
  level_value->set_text(
      std::to_string(currentGameState.level > 0 ? currentGameState.level : 0));
  score_value->set_text(std::to_string(currentGameState.score));
  high_score_value->set_text(std::to_string(currentGameState.high_score));
  return true;
}

void GUIView::SetupGameGrid() {
  game_canvas = Gtk::make_managed<Gtk::DrawingArea>();
  game_canvas->set_size_request(kFieldWidth * kBlockOnScreenSize,
                                kFieldHeight * kBlockOnScreenSize);
  game_canvas->set_draw_func([this](const Cairo::RefPtr<Cairo::Context>& cr,
                                    [[maybe_unused]] int width,
                                    [[maybe_unused]] int height) {
    auto currentGameState = updateCurrentState();
    cr->set_source_rgb(0.1f, 0.1f, 0.1f);
    cr->paint();
    for (int row = 0; row < kFieldHeight; ++row) {
      for (int col = 0; col < kFieldWidth; ++col) {
        const auto [r, g, b] = kColorMap.at(currentGameState.field[row][col]);
        cr->set_source_rgb(r, g, b);
        cr->rectangle(col * kBlockOnScreenSize + kBlockOnScreenMargin,
                      row * kBlockOnScreenSize + kBlockOnScreenMargin,
                      kBlockOnScreenSize - 2 * kBlockOnScreenMargin,
                      kBlockOnScreenSize - 2 * kBlockOnScreenMargin);
        cr->fill();
      }
    }
  });
  game_grid.attach(*game_canvas, 0, 0, 1, 1);
}

// Initialize the next board
void GUIView::SetupNextFigureGrid() {
  next_canvas = Gtk::make_managed<Gtk::DrawingArea>();
  next_canvas->set_size_request(kNextFieldHeight * kBlockOnScreenSize,
                                kNextFieldWidth * kBlockOnScreenSize);
  next_canvas->set_draw_func([this](const Cairo::RefPtr<Cairo::Context>& cr,
                                    [[maybe_unused]] int width,
                                    [[maybe_unused]] int height) {
    auto currentGameState = updateCurrentState();
    cr->set_source_rgb(0.1f, 0.1f, 0.1f);
    cr->paint();
    for (int row = 0; row < kNextFieldHeight; ++row) {
      for (int col = 0; col < kNextFieldWidth; ++col) {
        const auto [r, g, b] = kColorMap.at(
            currentGameState.next ? currentGameState.next[row][col] : 0);
        cr->set_source_rgb(r, g, b);
        cr->rectangle(col * kBlockOnScreenSize + kBlockOnScreenMargin,
                      row * kBlockOnScreenSize + kBlockOnScreenMargin,
                      kBlockOnScreenSize - 2 * kBlockOnScreenMargin,
                      kBlockOnScreenSize - 2 * kBlockOnScreenMargin);
        cr->fill();
      }
    }
  });

  next_grid.attach(*next_canvas, 0, 0, 1, 1);
}

// Initialize the info panel
void GUIView::SetupInfoPanel() {
  info_panel.set_orientation(Gtk::Orientation::VERTICAL);
  info_panel.set_valign(Gtk::Align::CENTER);
  info_panel.set_halign(Gtk::Align::CENTER);
  info_panel.set_spacing(20);
  info_panel.set_margin_top(20);
  info_panel.set_margin_bottom(20);
  info_panel.set_margin_start(20);
  info_panel.set_margin_end(20);

  auto currentGameState = updateCurrentState();
  // Level
  auto level_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto level_label = Gtk::make_managed<Gtk::Label>("LEVEL");
  level_value =
      Gtk::make_managed<Gtk::Label>(std::to_string(currentGameState.level));
  level_box->append(*level_label);
  level_box->append(*level_value);
  info_panel.append(*level_box);

  // Score
  auto score_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto score_label = Gtk::make_managed<Gtk::Label>("SCORE");
  score_value =
      Gtk::make_managed<Gtk::Label>(std::to_string(currentGameState.score));
  score_box->append(*score_label);
  score_box->append(*score_value);
  info_panel.append(*score_box);

  // High Score
  auto high_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto high_score_label = Gtk::make_managed<Gtk::Label>("HIGH");
  high_score_value = Gtk::make_managed<Gtk::Label>(
      std::to_string(currentGameState.high_score));
  high_box->append(*high_score_label);
  high_box->append(*high_score_value);
  info_panel.append(*high_box);

  // Next Figure
  auto next_fig_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto next_fig_label = Gtk::make_managed<Gtk::Label>("NEXT FIG.");
  next_fig_box->append(*next_fig_label);
  next_grid.set_row_homogeneous(true);
  next_grid.set_column_homogeneous(true);
  next_fig_box->append(next_grid);
  SetupNextFigureGrid();
  info_panel.append(*next_fig_box);

  // Instructions
  auto instruction_box =
      Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto instructions = Gtk::make_managed<Gtk::Label>(
      "s - Start\n"
      "p - Pause\n"
      "q - Quit\n"
      "arr. - Move\n"
      "___- Action\n");
  instruction_box->append(*instructions);
  info_panel.append(*instruction_box);
}

void GUIView::SetupTopStatusPanel() {
  top_status_panel.set_orientation(Gtk::Orientation::HORIZONTAL);
  top_status_panel.set_valign(Gtk::Align::CENTER);
  top_status_panel.set_halign(Gtk::Align::CENTER);
  top_status_panel.set_spacing(5);
  top_status_panel.set_margin_top(5);
  top_status_panel.set_margin_bottom(5);
  top_status_panel.set_margin_start(20);
  top_status_panel.set_margin_end(20);

  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(".bg { background-color:rgb(206, 255, 192); }");
  Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(),
                                              css_provider,
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
  top_status_panel.add_css_class("bg");
  auto message_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto message_lbl = Gtk::make_managed<Gtk::Label>("");
  message_box->append(*message_lbl);
  this->message_label = message_lbl;
  top_status_panel.append(*message_box);
}

bool GUIView::OnWindowKeyPressed(guint keyval, guint,
                                 [[maybe_unused]] Gdk::ModifierType state) {
  UserAction_t action;
  switch (keyval) {
    case GDK_KEY_s:
      action = UserAction_t::Start;
      break;
    case GDK_KEY_p:
      action = UserAction_t::Pause;
      break;
    case GDK_KEY_q:
      action = UserAction_t::Terminate;
      break;
    case GDK_KEY_Left:
      action = UserAction_t::Left;
      break;
    case GDK_KEY_Right:
      action = UserAction_t::Right;
      break;
    case GDK_KEY_Up:
      action = UserAction_t::Up;
      break;
    case GDK_KEY_Down:
      action = UserAction_t::Down;
      break;
    case GDK_KEY_space:
      action = UserAction_t::Action;
      break;
    default:
      return false;
  }
  if (action == UserAction_t::Terminate) {
    this->close();
    return false;
  }
  userInput(action, false);
  controller->UpdateCurrentState();
  return false;
}

std::string GUIView::GetStatusMessage() {
  std::string status_message = "";
  auto currentGameState = updateCurrentState();
  if (currentGameState.level == kStart) {
    status_message = "Press S to start";
  } else if (currentGameState.level == kWin) {
    status_message = "You win!";
  } else if (currentGameState.level == kLoose) {
    status_message = "You loose!";
  } else if (currentGameState.pause) {
    status_message = "Paused...";
  }
  return status_message;
}

}  // namespace s21