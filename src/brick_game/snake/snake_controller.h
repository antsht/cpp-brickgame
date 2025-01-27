
#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include "../controller.h"
#include "snake_model.h"

namespace s21 {

/**
 * @class SnakeController
 * @brief Handles user input and updates the game state for the Snake game.
 *
 * The SnakeController class is responsible for processing user input and
 * updating the game state of the Snake game. It interacts with the SnakeModel
 * class to manage the game logic.
 */
class SnakeController : public Controller {
 private:
  SnakeModel *model_;  ///< Pointer to the SnakeModel instance.

 public:
  /**
   * @brief Constructs a SnakeController object with the given SnakeModel.
   * @param model Pointer to the SnakeModel instance.
   */
  explicit SnakeController(SnakeModel *model);

  /**
   * @brief Updates the current game state.
   *
   * This method is called to update the current game state based on the user's
   * actions and the game logic.
   */
  void UpdateCurrentState() override;

  /**
   * @brief Processes the user's input and updates the game state accordingly.
   * @param action The user action to be processed.
   * @param hold Indicates whether the user is holding the action.
   */
  void processUserInput(UserAction_t action, bool hold) override;
};
}  // namespace s21

#endif  // SNAKE_CONTROLLER_H