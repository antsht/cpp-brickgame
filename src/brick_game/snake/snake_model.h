#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include <libgen.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common.h"

namespace s21 {

/**
 * @brief Enumeration representing the different states of the game.
 * - kStart: The game is in the starting state.
 * - kRunning: The game is currently running.
 * - kOnPause: The game is currently paused.
 * - kGameOver: The game has ended.
 */
enum class GameState { kStart, kRunning, kOnPause, kGameOver };
/**
 * @brief Enumeration representing the different directions the snake can move.
 * - kUp: The snake is moving upwards.
 * - kDown: The snake is moving downwards.
 * - kLeft: The snake is moving left.
 * - kRight: The snake is moving right.
 */
enum class SnakeDirection { kUp, kDown, kLeft, kRight };
/**
 * @brief Enumeration representing the different types of collisions that can
 * occur in the game.
 * - kNone: No collision has occurred.
 * - kWall: The snake has collided with a wall.
 * - kSnake: The snake has collided with itself.
 * - kApple: The snake has collided with an apple.
 */
enum class CollisionType { kNone, kWall, kSnake, kApple };

/**
 * @brief Enumeration representing the different colors used in the game.
 * - kBlack: Black color.
 * - kGreen: Green color.
 * - kRed: Red color.
 * - kYellow: Yellow color.
 * - kBlue: Blue color.
 * - kOrange: Orange color.
 * - kCyan: Cyan color.
 * - kPurple: Purple color.
 * - kWhite: White color.
 */
enum class Colors {
  kBlack = 0,
  kGreen = 1,
  kRed = 2,
  kYellow = 3,
  kBlue = 4,
  kOrange = 5,
  kCyan = 6,
  kPurple = 7,
  kWhite = 8
};

/**
 * @brief The initial delay between "automove" in milliseconds for the snake
 * game.
 */
constexpr int kInitialDelayInMs = 400;

/**
 * @brief The initial length of the snake in the game.
 */
constexpr int kInitialSnakeLength = 4;
/**
 * @brief The amount to reduce the delay between "automove" in milliseconds for
 * each level of the snake game.
 */
constexpr int kDelayReducePerLevelInMs = 25;

/**
 * @brief The maximum level that can be reached in the snake game.
 */
constexpr int kMaxLevel = 10;

/**
 * @brief The size of the snake that the player must reach to win the game.
 */
constexpr int kSnakeSizeToWin = kFieldHeight * kFieldWidth;

/**
 * @brief The name of the file that stores the high score for the Snake game.
 */
constexpr std::string_view kHighScoreFileName = "snake_high_score.txt";

using Cell = std::pair<int8_t, int8_t>;

/**
 * @brief The SnakeModel class represents the game logic for the Snake game.
 * It manages the state of the game, including the snake's position and
 * direction, the apple's position, the score, and the game state.
 */
class SnakeModel {
 public:
  /**
   * @brief Constructs a SnakeModel instance with the given runtime path.
   * @param runtime_path_ The path to the runtime directory.
   */
  explicit SnakeModel(const std::string &runtime_path_);

  /**
   * @brief Destructor for the SnakeModel class.
   */
  ~SnakeModel();

  SnakeModel(const SnakeModel &) = delete;
  SnakeModel &operator=(const SnakeModel &) = delete;
  SnakeModel(SnakeModel &&) = delete;
  SnakeModel &operator=(SnakeModel &&) = delete;

  /**
   * @brief Loads the high score from a file.
   */
  void LoadHighScore();

  /**
   * @brief Stores the current high score to a file.
   */
  void StoreHighScore();

  /**
   * @brief Updates the current state of the game based on user actions.
   */
  void UpdateCurrentState() noexcept;

  /**
   * @brief Handles the Finite State Machine (FSM) transitions based on user
   * actions.
   * @param action The user action to be processed.
   */
  void FSM(UserAction_t action) noexcept;

 private:
  static SnakeModel *instance;
  std::string runtime_path_;
  std::list<Cell> snake_;
  SnakeDirection direction_ = SnakeDirection::kUp;
  SnakeDirection next_direction_ = SnakeDirection::kUp;
  Cell apple_{0, 0};
  int score_{0};
  int high_score_{0};
  int level_{0};
  int speed_{1};
  GameState game_state_ = GameState::kStart;
  std::chrono::_V2::system_clock::time_point frame_start_in_ms_ =
      std::chrono::system_clock::now();
  std::mt19937 rand_engine_;
  std::uniform_int_distribution<size_t> random_int_distribution_;

  void MoveOneStepForward() noexcept;

  CollisionType CheckCollision(Cell next_head) noexcept;
  void GenerateApple() noexcept;
  void EatApple() noexcept;
  void UpdateScore() noexcept;

  void UpdateDirection() noexcept;
  void AllocateGameInfoField() noexcept;
  void DeallocateGameInfoField() noexcept;

 public:
  static GameInfo_t game_info;

  // for testing purposes
  friend class SnakeModelTest;
  friend class SnakeModelTest_Initialization_Test;
  friend class SnakeModelTest_LoadHighScore_Test;
  friend class SnakeModelTest_UpdateCurrentState_Test;
  friend class SnakeModelTest_UpdateDirection_Test;
  friend class SnakeModelTest_CheckCollision_Test;
  friend class SnakeModelTest_MoveOneStepForward_Test;
  friend class SnakeModelTest_EatApple_Test;
  friend class SnakeModelTest_GenerateApple_Test;
  friend class SnakeModelTest_FSMStateTransitions_Test;
  friend class SnakeModelTest_CheckWinGame_Test;
};

}  // namespace s21
#endif  // SNAKE_MODEL_H