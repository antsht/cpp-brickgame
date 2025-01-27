#include "snake_controller.h"

namespace s21 {

Controller* Controller::instance = nullptr;

SnakeController::SnakeController(SnakeModel* model) : model_(model) {
  if (instance != nullptr) {
    throw std::runtime_error("Controller instance already exists.");
  }
  instance = this;
};

void SnakeController::UpdateCurrentState() { model_->UpdateCurrentState(); }

void SnakeController::processUserInput(UserAction_t action,
                                       [[maybe_unused]] bool hold) {
  model_->FSM(action);
}

}  // namespace s21

GameInfo_t updateCurrentState() { return s21::SnakeModel::game_info; }

void userInput(UserAction_t action, bool hold) {
  if (s21::SnakeController::instance != nullptr) {
    s21::SnakeController::instance->processUserInput(action, hold);
  }
}
