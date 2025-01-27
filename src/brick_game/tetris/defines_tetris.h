#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

#define INTRO_MESSAGE "Press S to start!"
#define INTRO_MESSAGE_LEN 17
#define LEVEL_CNT 10

#define TETRAMINOS 7

#define BOARD_ROWS 20
#define BOARD_COLS 10

#define BOARDS_BEGIN 2

#define INITIAL_TIMEOUT_MS 1000
#define LEVELUP_TIMEOUT_STEP 80

#define DEFAULT_DELAY_MS 2000

#define BOARD_N (BOARD_ROWS + MAP_PADDING * 2 - 2)
#define BOARD_M (BOARD_COLS * 2 + MAP_PADDING * 2)
#define HUD_WIDTH 18
#define MAP_PADDING 1

#define BANNER_N 10
#define BANNER_M 100

#define SUCCESS 0
#define ERROR_T 1

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10

#define SCORE_1 100
#define SCORE_2 300
#define SCORE_3 700
#define SCORE_4 1500

#define MAX_LEVEL 10
#define FIGURE_PIXEL ' '
#define EMPTY_PIXEL ' '
#define BOARD_PIXEL ' '

// #define EMPTY_PAIR 1
// #define MOVING_PAIR 2
// #define STATIC_PAIR 3
// #define NEXT_PAIR 4

typedef enum {
  kColorBlack = 0,
  kColorGreen = 1,
  kColorRed = 2,
  kColorYellow = 3,
  kColorBlue = 4,
  kColorCyan = 5,
  kColorMagenta = 6,
  kColorWhite = 7
} colors_t;

#endif