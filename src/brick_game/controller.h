#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

namespace s21 {

/**
 * @brief Abstract base class for the game controller.
 *
 * The `Controller` class defines the interface for the game controller, which
 * is responsible for updating the current game state and processing user input.
 * Concrete implementations of this class should override the
 * `UpdateCurrentState()` and `processUserInput()` methods to provide the
 * desired game logic.
 */
class Controller {
 public:
  /**
   * Destructor for the `Controller` class.
   * This is the default destructor implementation.
   */
  virtual ~Controller() = default;
  /**
   * @brief Static instance of the `Controller` class.
   *
   * This static instance of the `Controller` class provides a global point of
   * access to the game controller. Concrete implementations of the `Controller`
   * class should set this instance to their own implementation.
   */
  static Controller *instance;
  /**
   * @brief Updates the current state of the game.
   *
   * This pure virtual method is responsible for updating the current state of
   * the game based on the game logic. Concrete implementations of the
   * `Controller` class should override this method to provide the desired game
   * state updates.
   */
  virtual void UpdateCurrentState() = 0;
  /**
   * @brief Processes user input for the game.
   *
   * This method is called to process a user action and update the game state
   * accordingly. The `action` parameter specifies the type of user action, and
   * the `hold` parameter indicates whether the action is being held down (e.g.
   * for continuous movement).
   *
   * @param action The type of user action to process.
   * @param hold Whether the action is being held down.
   */
  virtual void processUserInput(UserAction_t action, bool hold) = 0;
};
}  // namespace s21

#endif  // CONTROLLER_H