# BrickGame 2.0 Snake and Tetris

## Overview
This project is an implementation of the classic Snake and Tetris (C backend) games using C++ and the object-oriented programming paradigm. The game is developed as part of the BrickGame v2.0 project, which includes both a library for game logic and a desktop GUI. The project adheres to the MVC (Model-View-Controller) design pattern to ensure a clear separation of concerns between the game logic and the user interface.

## Features
- **Snake Game Mechanics**:
  - The snake moves continuously, and the player controls its direction using arrow keys.
  - The snake grows in length by one unit each time it eats an "apple".
  - The game ends if the snake collides with the boundaries of the playing field or itself.
  - The player wins when the snake reaches a length of 200 units.
  
- **GUI Integration**:
  - The game is integrated with a desktop GUI using either Qt or GTK+.
  - The GUI supports both the Snake game and the Tetris game from BrickGame v1.0.

- **Console Interface**:
  - The game logic is also accessible via a console interface, which supports the Snake game.

- **Scoring and Records (Bonus)**:
  - Players earn points by eating apples.
  - The highest score is saved and can be retrieved across game sessions.

- **Level Mechanics (Bonus)**:
  - The game includes level mechanics where the snake's speed increases every 5 points.
  - The maximum level is 10.

## Project Structure
- **Library Code**: Located in `src/brick_game/snake`, this contains the core game logic.
- **GUI Code**: Located in `src/gui/desktop`, this contains the desktop interface code.
- **Console Interface**: The console interface from BrickGame v1.0 is reused and supports the Snake game.

## Requirements
- **C++17**: The project is developed using the C++17 standard.
- **Google Style**: The code follows the Google C++ Style Guide.
- **Unit Testing**: The game logic is covered by unit tests using the GTest library, with a coverage of at least 80%.
- **Makefile**: The project is built using a Makefile with standard targets (`all`, `install`, `uninstall`, `clean`, `dvi`, `dist`, `test`).

## Design Patterns
- **MVC (Model-View-Controller)**: The project strictly follows the MVC pattern to separate the game logic (Model), the user interface (View), and the controller that mediates between them.
- **Finite State Machine (FSM)**: The game logic is formalized using a finite state machine to manage game states and transitions.

## Getting Started
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/antsht/cpp-brickgame.git
   cd cpp-brickgame/src
   make