#include <gtest/gtest.h>

#include "../snake/snake_model.h"

namespace s21 {

class SnakeModelTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test).

    model = new s21::SnakeModel(".");
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor).
    model->instance = nullptr;
    delete model;
  }

  s21::SnakeModel* model;
};

TEST_F(SnakeModelTest, Initialization) {
  EXPECT_EQ(model->score_, 0);
  EXPECT_EQ(model->snake_.size(), 4);
  EXPECT_EQ(model->snake_.front().first,
            (kFieldHeight - kInitialSnakeLength) / 2);
  EXPECT_EQ(model->snake_.front().second, kFieldWidth / 2);
  EXPECT_EQ(model->direction_, SnakeDirection::kUp);
  EXPECT_EQ(model->next_direction_, SnakeDirection::kUp);
  EXPECT_EQ(model->apple_.first >= 0, true);
  EXPECT_EQ(model->apple_.second >= 0, true);
  EXPECT_EQ(model->level_, 0);
  EXPECT_EQ(model->speed_, 1);
  EXPECT_EQ(model->game_state_, GameState::kStart);
}

TEST_F(SnakeModelTest, UpdateCurrentState) {
  model->snake_ = {{0, 0}, {0, 1}, {0, 2}};
  model->apple_ = {1, 1};
  model->score_ = 123;
  model->high_score_ = 666;
  model->level_ = 3;
  model->speed_ = 2;
  model->game_state_ = GameState::kRunning;
  model->UpdateCurrentState();
  EXPECT_EQ(model->game_info.field[0][0], 1);
  EXPECT_EQ(model->game_info.field[0][1], 1);
  EXPECT_EQ(model->game_info.field[0][2], 1);
  EXPECT_EQ(model->game_info.field[1][1], 2);
  EXPECT_EQ(model->game_info.score, 123);
  EXPECT_EQ(model->game_info.high_score, 666);
  EXPECT_EQ(model->game_info.level, 3);
  EXPECT_EQ(model->game_info.speed, 2);
  EXPECT_EQ(model->game_info.pause, false);
}

TEST_F(SnakeModelTest, UpdateDirection) {
  model->direction_ = SnakeDirection::kUp;
  model->next_direction_ = SnakeDirection::kDown;
  model->UpdateDirection();
  EXPECT_EQ(model->direction_, SnakeDirection::kUp);

  model->next_direction_ = SnakeDirection::kLeft;
  model->UpdateDirection();
  EXPECT_EQ(model->direction_, SnakeDirection::kLeft);

  model->next_direction_ = SnakeDirection::kRight;
  model->UpdateDirection();
  EXPECT_EQ(model->direction_, SnakeDirection::kLeft);

  model->direction_ = SnakeDirection::kDown;
  model->next_direction_ = SnakeDirection::kUp;
  model->UpdateDirection();
  EXPECT_EQ(model->direction_, SnakeDirection::kDown);

  model->next_direction_ = SnakeDirection::kRight;
  model->UpdateDirection();
  EXPECT_EQ(model->direction_, SnakeDirection::kRight);
}

TEST_F(SnakeModelTest, CheckCollision) {
  model->snake_ = {{0, 0}, {0, 1}, {0, 2}};
  model->apple_ = {1, 2};
  Cell nextHead = {0, 3};
  EXPECT_EQ(model->CheckCollision(nextHead), CollisionType::kNone);
  nextHead = {0, 2};
  EXPECT_EQ(model->CheckCollision(nextHead), CollisionType::kSnake);
  nextHead = {-1, 2};
  EXPECT_EQ(model->CheckCollision(nextHead), CollisionType::kWall);
  nextHead = {1, 2};
  EXPECT_EQ(model->CheckCollision(nextHead), CollisionType::kApple);
}

TEST_F(SnakeModelTest, FSMStateTransitions) {
  EXPECT_EQ(model->game_state_, GameState::kStart);
  model->FSM(UserAction_t::Start);
  EXPECT_EQ(model->game_state_, GameState::kRunning);
  model->FSM(UserAction_t::Pause);
  EXPECT_EQ(model->game_state_, GameState::kOnPause);
  model->FSM(UserAction_t::Pause);
  EXPECT_EQ(model->game_state_, GameState::kRunning);

  auto initial_pos = model->snake_.front();
  model->apple_ = {(kFieldHeight - kInitialSnakeLength) / 2 -1, kFieldWidth / 2};
  auto apple_pos = model->apple_;
  model->FSM(UserAction_t::Action);
  auto pos_after_move = model->snake_.front();
  EXPECT_NE(initial_pos, pos_after_move);
  EXPECT_NE(apple_pos, model->apple_);

  model->FSM(UserAction_t::Left);
  EXPECT_EQ(model->next_direction_, SnakeDirection::kLeft);

  model->FSM(UserAction_t::Action);
  model->game_state_ = GameState::kRunning;
  model->FSM(UserAction_t::Down);
  EXPECT_EQ(model->next_direction_, SnakeDirection::kDown);

  model->FSM(UserAction_t::Action);
  model->game_state_ = GameState::kRunning;
  model->FSM(UserAction_t::Right);
  EXPECT_EQ(model->next_direction_, SnakeDirection::kRight);

  model->FSM(UserAction_t::Action);
  model->game_state_ = GameState::kRunning;
  model->FSM(UserAction_t::Up);
  EXPECT_EQ(model->next_direction_, SnakeDirection::kUp);

  model->FSM(UserAction_t::Action);
  model->game_state_ = GameState::kRunning;
  model->FSM(UserAction_t::Terminate);
  EXPECT_EQ(model->game_state_, GameState::kGameOver);
}

TEST_F(SnakeModelTest, CheckWinGame) {
  // fill mock snake with 199 cells
  model->snake_.clear();
  for (int i = 1; i < 200; i++) {
      model->snake_.emplace_back(i, 0);
    }
  std::pair<int,int> snake_head = model->snake_.front();
  EXPECT_EQ(snake_head.first, 1);
  EXPECT_EQ(snake_head.second, 0);
  model->score_ = 199;
  model->high_score_ = 199 ;
  model->next_direction_ = SnakeDirection::kUp;
  model->apple_ = {0, 0};
  model->game_state_ = GameState::kRunning;
  model->FSM(UserAction_t::Action);
  EXPECT_EQ(model->game_state_, GameState::kGameOver);
  EXPECT_EQ(model->level_, kWin);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace s21
