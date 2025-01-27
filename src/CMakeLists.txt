cmake_minimum_required(VERSION 3.14)

project(BrickGame LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build directories
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(OBJ_DIR ${BUILD_DIR}/obj)
set(LIB_DIR ${BUILD_DIR}/lib)
set(TEST_DIR ${BUILD_DIR}/tests)
set(INSTALL_DIR ${CMAKE_BINARY_DIR}/bin)

# Include required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

# Include directories
include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${SRC_DIR}
)

# Link directories
link_directories(${GTKMM_LIBRARY_DIRS})
# Define source groups
file(GLOB SNAKE_BACKEND_SRCS ${SRC_DIR}/brick_game/snake/*.cc)
file(GLOB TETRIS_BACKEND_SRCS ${SRC_DIR}/brick_game/tetris/*.c ${SRC_DIR}/brick_game/tetris/*.cc)
file(GLOB CONSOLE_SRCS ${SRC_DIR}/gui/console/*.cc)
file(GLOB GUI_SRCS ${SRC_DIR}/gui/desktop/*.cc)
file(GLOB TEST_SRCS ${SRC_DIR}/brick_game/tests/*.cc)

# Add libraries
add_library(snake_lib STATIC ${SNAKE_BACKEND_SRCS})
set_source_files_properties(${TETRIS_BACKEND_SRCS} PROPERTIES LANGUAGE CXX)
add_library(tetris_lib STATIC ${TETRIS_BACKEND_SRCS})

# Console applications
add_executable(snakeConsole ${CONSOLE_SRCS} ${SRC_DIR}/console_snake.cc)
target_link_libraries(snakeConsole snake_lib ncurses)

add_executable(tetrisConsole ${CONSOLE_SRCS} ${SRC_DIR}/console_tetris.cc)
target_link_libraries(tetrisConsole tetris_lib ncurses)

# GUI applications
add_executable(snakeGUI ${GUI_SRCS} ${SRC_DIR}/GUI_snake.cc)
target_link_libraries(snakeGUI snake_lib ${GTKMM_LIBRARIES})

add_executable(tetrisGUI ${GUI_SRCS} ${SRC_DIR}/GUI_tetris.cc)
target_link_libraries(tetrisGUI tetris_lib ${GTKMM_LIBRARIES})

# Tests
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# add_executable(tests ${TEST_SRCS} ${SNAKE_BACKEND_SRCS})
# target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

# Test coverage target (optional)
# option(ENABLE_COVERAGE "Enable test coverage" OFF)
# if(ENABLE_COVERAGE)
#    target_compile_options(tests PRIVATE --coverage)
#    target_link_libraries(tests PRIVATE gcov)
#endif()

# Installation
install(TARGETS snakeConsole tetrisConsole snakeGUI tetrisGUI
    RUNTIME DESTINATION ${INSTALL_DIR}
)

# Distribution package
set(DIST_NAME "brick_game.tar.gz")
add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E tar czf ${DIST_NAME} --
        ${SRC_DIR}/brick_game
        ${SRC_DIR}/gui
        ${SRC_DIR}/console_snake.cc
        ${SRC_DIR}/console_tetris.cc
        ${SRC_DIR}/GUI_snake.cc
        ${SRC_DIR}/GUI_tetris.cc
        ${SRC_DIR}/Makefile
        ${SRC_DIR}/Doxyfile
)

# Formatting checks
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(check
        COMMAND find ${SRC_DIR} -regex ".*\\.(cc|hpp|c|h)" -exec ${CLANG_FORMAT} -style=Google -n {} \;
    )

    add_custom_target(check_fix
        COMMAND find ${SRC_DIR} -regex ".*\\.(cc|hpp|c|h)" -exec ${CLANG_FORMAT} -style=Google -i {} \;
    )
endif()

# Clean target
add_custom_target(clean_build_dir
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BUILD_DIR}
)
