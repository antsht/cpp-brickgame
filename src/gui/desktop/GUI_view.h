/**
 * @file GUI_view.h
 * @brief Defines the GUIView class, which is responsible for the graphical user
 * interface of the brick game application.
 *
 * The GUIView class inherits from Gtk::Window and manages the layout and
 * rendering of the game board, next figure preview, and various informational
 * panels. It also handles keyboard input events and updates the display based
 * on the game state provided by the Controller class.
 */
#ifndef GUIVIEW_H
#define GUIVIEW_H

#include <gtkmm.h>

#include <map>
#include <tuple>

#include "../../brick_game/controller.h"

namespace s21 {

static constexpr uint16_t kWindowWidth = 300;
static constexpr uint16_t kWindowHeigth = 400;
static constexpr uint8_t kBlockOnScreenSize = 20;
static constexpr uint8_t kBlockOnScreenMargin = 1;
static constexpr uint8_t kDrawTimeoutInMS = 25;

const std::map<int, std::tuple<float, float, float>> kColorMap = {
    // Format: {index, {R, G, B}},
    {0, {0.0f, 0.0f, 0.0f}},                                 // Black
    {1, {70.0f / 255.0f, 198.0f / 255.0f, 44.0f / 255.0f}},  // Green
    {2, {1.0f, 0.0f, 0.0f}},                                 // Red
    {3, {1.0f, 1.0f, 0.0f}},                                 // Yellow
    {4, {0.0f, 0.0f, 1.0f}},                                 // Blue
    {5, {0.0f, 200.0f / 255.0f, 1.0f}},                      // Cyan
    {6, {1.0f, 0.0f, 1.0f}},                                 // Magenta
    {7, {0.9f, 0.9f, 0.9f}},                                 // White
};

class GUIView : public Gtk::Window {
 private:
  s21::Controller *controller;
  Gtk::Box outer_box;
  Gtk::Box main_box;          // Main container
  Gtk::Grid game_grid;        // Game board grid (20x10)
  Gtk::Grid next_grid;        // Next fig. grid (4x4)
  Gtk::Box info_panel;        // Right-side info panel
  Gtk::Box top_status_panel;  // Bottom info panel
  Gtk::DrawingArea *game_canvas = nullptr;
  Gtk::DrawingArea *next_canvas = nullptr;
  Gtk::Label *message_label = nullptr;
  Gtk::Label *level_value = nullptr;
  Gtk::Label *score_value = nullptr;
  Gtk::Label *high_score_value = nullptr;

  bool OnWindowKeyPressed(guint keyval, guint, Gdk::ModifierType state);
  bool OnTimeout();

 public:
  explicit GUIView(s21::Controller *c);
  GUIView() = delete;
  ~GUIView() = default;

 private:
  void SetupGameGrid();
  void SetupNextFigureGrid();
  void SetupInfoPanel();
  void SetupTopStatusPanel();
  std::string GetStatusMessage();
};

}  // namespace s21
#endif