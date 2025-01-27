#include "console_view.h"

#include <iostream>

namespace s21 {

void BrickGameConsoleView::PrintOverlay(void) {
  PrintFrame(0, kBoardN + 1, 0, kBoardM + 1);
  PrintFrame(0, kBoardN + 1, kBoardM + 2, kBoardM + kHUDWidth + 3);

  PrintFrame(1, 3, kBoardM + 3, kBoardM + kHUDWidth + 2);
  PrintFrame(4, 6, kBoardM + 3, kBoardM + kHUDWidth + 2);
  PrintFrame(7, 9, kBoardM + 3, kBoardM + kHUDWidth + 2);
  PrintFrame(10, 15, kBoardM + 3, kBoardM + kHUDWidth + 2);

  s21::NCursesWrapper::ncPrintW(2, kBoardM + 5, "LEVEL");
  s21::NCursesWrapper::ncPrintW(5, kBoardM + 5, "SCORE");
  s21::NCursesWrapper::ncPrintW(8, kBoardM + 5, "HIGH");

  s21::NCursesWrapper::ncPrintW(11, kBoardM + 5, "NEXT");
  s21::NCursesWrapper::ncPrintW(12, kBoardM + 5, "FIG.");

  s21::NCursesWrapper::ncPrintW(16, kBoardM + 6, "s  - Start");
  s21::NCursesWrapper::ncPrintW(17, kBoardM + 6, "p  - Pause");
  s21::NCursesWrapper::ncPrintW(18, kBoardM + 6, "q  - Quit");
  s21::NCursesWrapper::ncPrintW(19, kBoardM + 6, "_  - Action");
  s21::NCursesWrapper::ncPrintW(20, kBoardM + 6, "ar - Move");
}

void BrickGameConsoleView::PrintFrame(int top_y, int bottom_y, int left_x,
                                      int right_x) {
  s21::NCursesWrapper::ncAddWChar(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) {
    s21::NCursesWrapper::ncAddWChar(top_y, i, ACS_HLINE);
  }
  s21::NCursesWrapper::ncAddWChar(top_y, i, ACS_URCORNER);

  for (int k = top_y + 1; k < bottom_y; k++) {
    s21::NCursesWrapper::ncAddWChar(k, left_x, ACS_VLINE);
    s21::NCursesWrapper::ncAddWChar(k, right_x, ACS_VLINE);
  }

  s21::NCursesWrapper::ncAddWChar(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) {
    s21::NCursesWrapper::ncAddWChar(bottom_y, i, ACS_HLINE);
  }
  s21::NCursesWrapper::ncAddWChar(bottom_y, i, ACS_LRCORNER);
}

void BrickGameConsoleView::PrintGameBoard(const GameInfo_t &current_game_info) {
  for (int i = 0; i < kBoardRows; ++i)
    for (int j = 0; j < kBoardCols; ++j) {
      s21::NcColorPairs color =
          static_cast<s21::NcColorPairs>(current_game_info.field[i][j]);
      attron(COLOR_PAIR(color));
      s21::NCursesWrapper::ncAddWChar(kBoardOffset + i - 1,
                                      kBoardOffset + 2 * j, kEmptyPixel);
      s21::NCursesWrapper::ncAddWChar(kBoardOffset + i - 1,
                                      kBoardOffset + 2 * j + 1, kEmptyPixel);
      attroff(COLOR_PAIR(color));
    }
}

void BrickGameConsoleView::PrintNextFigure(
    const GameInfo_t &current_game_info) {
  if (current_game_info.next == nullptr) {
    return;
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      s21::NcColorPairs color =
          static_cast<s21::NcColorPairs>(current_game_info.next[i][j]);
      attron(COLOR_PAIR(color));
      s21::NCursesWrapper::ncAddWChar(11 + i, kBoardM + 12 + 2 * j,
                                      kEmptyPixel);
      s21::NCursesWrapper::ncAddWChar(11 + i, kBoardM + 12 + 2 * j + 1,
                                      kEmptyPixel);
      attroff(COLOR_PAIR(color));
    }
  }
}

void BrickGameConsoleView::PrintStats(const GameInfo_t &current_game_info) {
  s21::NCursesWrapper::ncPrintW(
      2, kBoardM + 12, "%d",
      current_game_info.level > 0 ? current_game_info.level : 0);
  s21::NCursesWrapper::ncPrintW(5, kBoardM + 12, "%d", current_game_info.score);
  s21::NCursesWrapper::ncPrintW(8, kBoardM + 12, "%d",
                                current_game_info.high_score);
}

void BrickGameConsoleView::PrintBanner(std::string &&banner_text) {
  int len = banner_text.length();
  PrintFrame(kBoardRows / 2 - 1, kBoardRows / 2 + 1, kBoardCols + 1 - len / 2,
             kBoardCols + 2 - len / 2 + len);
  s21::NCursesWrapper::ncPrintW(kBoardRows / 2, kBoardCols + 2 - len / 2, "%s",
                                banner_text.c_str());
}

std::optional<UserAction_t> BrickGameConsoleView::GetUserInput() {
  std::optional<UserAction_t> input;
  int ch = getch();
  switch (ch) {
    case 's':
      input = UserAction_t::Start;
      break;
    case KEY_UP:
      input = UserAction_t::Up;
      break;
    case KEY_DOWN:
      input = UserAction_t::Down;
      break;
    case KEY_LEFT:
      input = UserAction_t::Left;
      break;
    case KEY_RIGHT:
      input = UserAction_t::Right;
      break;
    case 'p':
      input = UserAction_t::Pause;
      break;
    case 'q':
      input = UserAction_t::Terminate;
      break;
    case ' ':
      input = UserAction_t::Action;
      break;
    default:
      input = std::nullopt;
      break;
  }
  return input;
}

void BrickGameConsoleView::StartEventLoop() {
  NCursesWrapper nc;
  NcInit(10);
  controller->UpdateCurrentState();

  // auto frame_start_in_ms = std::chrono::system_clock::now();

  while (true) {
    PrintOverlay();
    nc.refresh();
    auto input = GetUserInput();
    if (input) {
      if (input.value() == UserAction_t::Terminate) {
        break;
      }
      userInput(input.value(), false);
    }
    controller->UpdateCurrentState();
    GameInfo_t current_game_info = updateCurrentState();
    PrintGameBoard(current_game_info);
    PrintNextFigure(current_game_info);
    PrintStats(current_game_info);
    if (current_game_info.level == 0) {
      PrintBanner(std::string{"'s' to start"});
    }
    if (current_game_info.pause) {
      PrintBanner(std::string{"PAUSED"});
    } else if (current_game_info.level == kLoose) {
      PrintBanner(std::string{"GAME OVER"});
    } else if (current_game_info.level == kWin) {
      PrintBanner(std::string{"YOU WIN"});
    }
  }
}

void NcInit(int time) {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(time);

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  setlocale(LC_ALL, "");

  start_color();
  init_pair(kWhiteBlackPair, COLOR_WHITE, COLOR_BLACK);
  init_pair(kWhiteGreenPair, COLOR_WHITE, COLOR_GREEN);
  init_pair(kWhiteRedPair, COLOR_WHITE, COLOR_RED);
  init_pair(kWhiteYellowPair, COLOR_WHITE, COLOR_YELLOW);
  init_pair(kWhiteBluePair, COLOR_WHITE, COLOR_BLUE);
  init_pair(kWhiteCyanPair, COLOR_WHITE, COLOR_CYAN);
  init_pair(kWhiteMagentaPair, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(kWhiteWhitePair, COLOR_WHITE, COLOR_WHITE);
}

}  // namespace s21