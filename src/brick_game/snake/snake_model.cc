#include "snake_model.h"

// namespace s21
namespace s21 {

GameInfo_t SnakeModel::game_info{};
SnakeModel *SnakeModel::instance = nullptr;

SnakeModel::SnakeModel(const std::string &runtime_path_)
    : runtime_path_(runtime_path_), rand_engine_(std::random_device{}()) {
  // LCOV_EXCL_START
  if (instance != nullptr) {
    throw std::runtime_error("Model instance already exists.");
  }
  // LCOV_EXCL_STOP
  instance = this;

  for (int i = 0; i < kInitialSnakeLength; ++i) {
    snake_.emplace_back(                               // LCOV_EXCL_LINE
        (kFieldHeight - kInitialSnakeLength) / 2 + i,  // LCOV_EXCL_LINE
        kFieldWidth / 2);                              // LCOV_EXCL_LINE
  }

  game_info.next = nullptr;
  game_info.speed = 1;

  LoadHighScore();
  game_info.high_score = high_score_;

  AllocateGameInfoField();

  GenerateApple();
}

SnakeModel::~SnakeModel() { DeallocateGameInfoField(); }

void SnakeModel::AllocateGameInfoField() noexcept {
  try {
    game_info.field = new int *[kFieldHeight];
    for (int i = 0; i < kFieldHeight; i++) {
      game_info.field[i] = new int[kFieldWidth]{0};
    }
    // LCOV_EXCL_START
  } catch (std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
  }
  // LCOV_EXCL_STOP
}

void SnakeModel::DeallocateGameInfoField() noexcept {
  for (int i = 0; i < kFieldHeight; i++) {
    delete[] game_info.field[i];
  }
  delete[] game_info.field;
}

void SnakeModel::LoadHighScore() {
  std::filesystem::path score_path =
      std::filesystem::path(runtime_path_) / kHighScoreFileName;
  if (std::ifstream fs(score_path); fs.is_open()) {
    fs >> high_score_;
  }
}

void SnakeModel::StoreHighScore() {
  std::filesystem::path score_path =
      std::filesystem::path(runtime_path_) / kHighScoreFileName;

  if (std::ofstream fs(score_path); fs.is_open()) {
    fs << high_score_;
  } else {
    // LCOV_EXCL_START
    std::cerr << "Error: Unable to open snake_high_score.txt" << std::endl;
    // LCOV_EXCL_STOP
  }
}

void SnakeModel::UpdateCurrentState() noexcept {
  auto current_time_in_ms = std::chrono::system_clock::now();
  auto elapsed_time_in_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time_in_ms -
                                                            frame_start_in_ms_)
          .count();
  // LCOV_EXCL_START
  if (elapsed_time_in_ms >
      (kInitialDelayInMs -
       kDelayReducePerLevelInMs * updateCurrentState().level)) {
    frame_start_in_ms_ = current_time_in_ms;
    userInput(UserAction_t::Action, false);
  }
  // LCOV_EXCL_STOP
  for (int i = 0; i < kFieldHeight; ++i) {
    std::fill(game_info.field[i], game_info.field[i] + kFieldWidth, 0);
  }

  for (const auto &[row, col] : snake_) {
    game_info.field[row][col] = static_cast<int>(Colors::kGreen);
  }

  game_info.field[apple_.first][apple_.second] = static_cast<int>(Colors::kRed);
  game_info.score = score_;
  game_info.high_score = high_score_;
  game_info.level = level_;
  game_info.speed = speed_;
  game_info.pause = game_state_ == GameState::kOnPause;
}

void SnakeModel::UpdateDirection() noexcept {
  // Define the opposite direction of each possible direction
  static const std::unordered_map<SnakeDirection, SnakeDirection> opposites = {
      {SnakeDirection::kUp, SnakeDirection::kDown},
      {SnakeDirection::kDown, SnakeDirection::kUp},
      {SnakeDirection::kLeft, SnakeDirection::kRight},
      {SnakeDirection::kRight, SnakeDirection::kLeft},
  };

  // Update direction only if the next direction is not the opposite of the
  // current one
  if (next_direction_ != opposites.at(direction_)) {
    direction_ = next_direction_;
  }

  // Ensure next_direction_ always aligns with direction_
  next_direction_ = direction_;
}

void SnakeModel::FSM(UserAction_t action) noexcept {
  switch (action) {
    case Start:
      if (game_state_ == GameState::kStart) {
        game_state_ = GameState::kRunning;
        level_ = kLevel1;
      }
      break;
    case Pause:
      if (game_state_ == GameState::kRunning) {
        game_state_ = GameState::kOnPause;
      } else if (game_state_ == GameState::kOnPause) {
        game_state_ = GameState::kRunning;
      }
      break;
    case Terminate:
      game_state_ = GameState::kGameOver;
      break;
    case Left:
      next_direction_ = SnakeDirection::kLeft;
      break;
    case Right:
      next_direction_ = SnakeDirection::kRight;
      break;
    case Up:
      next_direction_ = SnakeDirection::kUp;
      break;
    case Down:
      next_direction_ = SnakeDirection::kDown;
      break;
    case Action:
      if (game_state_ == GameState::kRunning) {
        if (frame_start_in_ms_ < std::chrono::system_clock::now()) {
          MoveOneStepForward();
          frame_start_in_ms_ =
              std::chrono::system_clock::now() + std::chrono::milliseconds(50);
        }
      }
      break;
    default:  // LCOV_EXCL_LINE
      break;  // LCOV_EXCL_LINE
  }
}
CollisionType SnakeModel::CheckCollision(Cell next_head) noexcept {
  // Check for wall collision
  if (next_head.first < 0 || next_head.first >= kFieldHeight ||
      next_head.second < 0 || next_head.second >= kFieldWidth) {
    return CollisionType::kWall;
  }

  // Check for snake collision
  if (std::any_of(
          snake_.begin(), snake_.end(),
          [&next_head](const auto &segment) { return segment == next_head; })) {
    return CollisionType::kSnake;
  }

  // Check for apple collision
  if (next_head == apple_) {
    return CollisionType::kApple;
  }

  // No collision
  return CollisionType::kNone;
}

void SnakeModel::MoveOneStepForward() noexcept {
  if (game_state_ != GameState::kRunning) {
    return;
  }
  UpdateDirection();
  auto head = snake_.front();
  Cell newHead = head;
  switch (direction_) {
    case SnakeDirection::kUp:
      newHead.first--;
      break;
    case SnakeDirection::kDown:
      newHead.first++;
      break;
    case SnakeDirection::kLeft:
      newHead.second--;
      break;
    case SnakeDirection::kRight:
      newHead.second++;
      break;
    default:
      break;
  }

  auto collision = CheckCollision(newHead);
  switch (collision) {
    case CollisionType::kWall:
    case CollisionType::kSnake:
      game_state_ = GameState::kGameOver;
      level_ = kLoose;
      return;
    case CollisionType::kApple:
      EatApple();
      return;
    case CollisionType::kNone:
      snake_.push_front(newHead);
      snake_.pop_back();
      return;
  }
}

void SnakeModel::EatApple() noexcept {
  snake_.push_front(apple_);
  score_++;
  if (score_ > high_score_) {
    high_score_ = score_;
    StoreHighScore();
  }
  if (score_ % 5 == 0 && level_ < kMaxLevel) {
    level_++;
  }
  if (snake_.size() == kSnakeSizeToWin) {
    game_state_ = GameState::kGameOver;
    level_ = kWin;
  } else {
    GenerateApple();
  }
}

void SnakeModel::GenerateApple() noexcept {
  std::vector<Cell> empty_cells;
  empty_cells.reserve(kFieldHeight * kFieldWidth);

  for (int i = 0; i < kFieldHeight; i++) {
    for (int j = 0; j < kFieldWidth; j++) {
      if (game_info.field[i][j] == 0) {
        empty_cells.push_back({i, j});
      }
    }
  }

  if (empty_cells.empty()) {
    game_state_ = GameState::kGameOver;
    level_ = kWin;
  } else {
    std::uniform_int_distribution random_int_distribution{
        0UL, empty_cells.size() - 1};  // Generate a random index
    size_t idx = random_int_distribution(rand_engine_);
    apple_ = empty_cells[idx];
  }
}

}  // namespace s21
