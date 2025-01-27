#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <libgen.h>
#include <ncurses.h>

#include <chrono>
#include <iostream>
#include <optional>

#include "../../brick_game/common.h"
#include "../../brick_game/controller.h"

namespace s21 {
constexpr int kBoardOffset = 2;

constexpr int kHUDWidth = 18;
constexpr int kMapPadding = 1;

constexpr int kBoardRows = 20;
constexpr int kBoardCols = 10;

constexpr int kBoardN = (kBoardRows + kMapPadding * 2 - 2);
constexpr int kBoardM = (kBoardCols * 2 + kMapPadding * 2);

enum NcColorPairs {
  kWhiteBlackPair = 0,
  kWhiteGreenPair,
  kWhiteRedPair,
  kWhiteYellowPair,
  kWhiteBluePair,
  kWhiteCyanPair,
  kWhiteMagentaPair,
  kWhiteWhitePair
};

constexpr char kBoardPixel = ' ';
constexpr char kEmptyPixel = ' ';
void NcInit(int time);

class NCursesWrapper {
 public:
  NCursesWrapper() { initscr(); }
  ~NCursesWrapper() { endwin(); }
  void refresh() { ::refresh(); }

  static inline void ncPrintW(int y, int x, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    wmove(stdscr, kBoardOffset + y, kBoardOffset + x);
    vw_printw(stdscr, fmt, args);
    va_end(args);
  }
  static inline void ncAddWChar(int y, int x, wchar_t c) {
    mvaddch(kBoardOffset + y, kBoardOffset + x, c);
  }
};

class BrickGameConsoleView {
 private:
  Controller *controller;

 public:
  explicit BrickGameConsoleView(Controller *c) : controller(c) {}
  void StartEventLoop();

  std::optional<UserAction_t> GetUserInput();

  void PrintOverlay(void);
  void PrintFrame(int top_y, int bottom_y, int left_x, int right_x);
  void PrintGameBoard(const GameInfo_t &current_game_info);
  void PrintNextFigure(const GameInfo_t &current_game_info);
  void PrintStats(const GameInfo_t &current_game_info);
  void PrintBanner(std::string &&banner_text);
};

}  // namespace s21
#endif