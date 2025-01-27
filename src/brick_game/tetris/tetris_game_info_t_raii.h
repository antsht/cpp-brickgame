
#ifndef TETRIS_GAME_INFO_T_RAII_H
#define TETRIS_GAME_INFO_T_RAII_H

#include "../common.h"

class GameInfo {
 public:
  GameInfo() {
    info_.field = new int*[kFieldHeight];
    for (int i = 0; i < kFieldHeight; i++) {
      info_.field[i] = new int[kFieldWidth];
    }

    info_.next = new int*[kNextFieldHeight];
    for (int i = 0; i < kNextFieldHeight; i++) {
      info_.next[i] = new int[kNextFieldWidth];
    }

    info_.pause = 0;
    info_.score = 0;
    info_.high_score = 0;
    info_.level = 0;
    info_.speed = 1;
  }

  ~GameInfo() {
    for (int i = 0; i < kFieldHeight; i++) {
      delete[] info_.field[i];
    }
    delete[] info_.field;

    for (int i = 0; i < kNextFieldHeight; i++) {
      delete[] info_.next[i];
    }
    delete[] info_.next;
  }

  GameInfo_t* get() { return &info_; }

 private:
  GameInfo_t info_;
};

#endif  // TETRIS_GAME_INFO_T_RAII_H