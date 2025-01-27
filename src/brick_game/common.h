#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

static constexpr uint8_t kFieldWidth = 10;
static constexpr uint8_t kFieldHeight = 20;
static constexpr uint8_t kNextFieldWidth = 4;
static constexpr uint8_t kNextFieldHeight = 4;

typedef enum {
  kStart = 0,
  kWin = -1,
  kLoose = -2,
  kLevel1 = 1,
  kLevel2 = 2,
  kLevel3 = 3,
  kLevel4 = 4,
  kLevel5 = 5,
  kLevel6 = 6,
  kLevel7 = 7,
  kLevel8 = 8,
  kLevel9 = 9,
  kLevel10 = 10
} GameState_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

#endif  // COMMON_H