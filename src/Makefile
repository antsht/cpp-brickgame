#########################################
#--------- Required GUI libs -----------#
#########################################
#Ubuntu 24.04
#sudo apt install libgtk-4-dev
#sudo apt install libgtkmm-4.0-dev 

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra 
LDFLAGS_CONSOLE = -lncurses
GTKMMFLAGS = `pkg-config --cflags gtkmm-4.0`
GTKMMLIBS = `pkg-config --libs gtkmm-4.0`
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)/lib
TEST_DIR = $(BUILD_DIR)/tests
INSTALL_DIR = bin
DIST_DIR = dist
DIST_NAME = brick_game.tar.gz
SNAKE_LIB_NAME = libsnake.a
TETRIS_LIB_NAME = libtetris.a

SNAKE_BACKEND_SRCS = $(wildcard ./brick_game/snake/*.cc)
SNAKE_BACKEND_OBJS = $(patsubst ./%.cc,$(OBJ_DIR)/%.o,$(SNAKE_BACKEND_SRCS))

TETRIS_BACKEND_SRCS = $(wildcard ./brick_game/tetris/*.c*)
TETRIS_BACKEND_OBJS = $(patsubst ./%.c,$(OBJ_DIR)/%.o,$(filter %.c,$(TETRIS_BACKEND_SRCS))) \
                      $(patsubst ./%.cc,$(OBJ_DIR)/%.o,$(filter %.cc,$(TETRIS_BACKEND_SRCS)))

CONSOLE_SRCS = $(wildcard ./gui/console/*.cc)
GUI_SRCS = $(wildcard ./gui/desktop/*.cc)
TEST_SRCS = $(wildcard ./brick_game/tests/*.cc)


#########################################
#--------- Build all binaries ----------#
#########################################
.PHONY: all clean test snake_lib tetris_lib 
all: clean console GUI test

console: snake_lib tetris_lib
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CONSOLE_SRCS) console_snake.cc $(LIB_DIR)/$(SNAKE_LIB_NAME) -o $(BUILD_DIR)/snakeConsole $(LDFLAGS_CONSOLE)
	$(CXX) $(CXXFLAGS) $(CONSOLE_SRCS) console_tetris.cc $(LIB_DIR)/$(TETRIS_LIB_NAME) -o $(BUILD_DIR)/tetrisConsole $(LDFLAGS_CONSOLE)
	rm -rf $(OBJ_DIR)

GUI: snake_lib tetris_lib
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(GUI_SRCS) GUI_snake.cc $(GTKMMFLAGS) $(LIB_DIR)/$(SNAKE_LIB_NAME) -o $(BUILD_DIR)/snakeGUI $(GTKMMLIBS)
	$(CXX) $(CXXFLAGS) $(GUI_SRCS) GUI_tetris.cc $(GTKMMFLAGS) $(LIB_DIR)/$(TETRIS_LIB_NAME) -o $(BUILD_DIR)/tetrisGUI $(GTKMMLIBS)

test: snake_lib
	@mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(LIB_DIR)/$(SNAKE_LIB_NAME) -lgtest -lgtest_main -pthread -o $(TEST_DIR)/$@
	./$(TEST_DIR)/$@
	rm -rf ./*score.txt

snake_lib: $(LIB_DIR)/$(SNAKE_LIB_NAME)

$(LIB_DIR)/$(SNAKE_LIB_NAME): $(SNAKE_BACKEND_OBJS) $(COMMON_OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

tetris_lib: $(LIB_DIR)/$(TETRIS_LIB_NAME)

$(LIB_DIR)/$(TETRIS_LIB_NAME): $(TETRIS_BACKEND_OBJS) $(COMMON_OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

# Rule for C files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for C++ files
$(OBJ_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#########################################
#----------- Test coverage -------------#
#########################################
.PHONY: gcov_report
gcov_report: snake_lib
	@mkdir -p $(TEST_DIR)
	$(CXX) --coverage $(CXXFLAGS)  $(SNAKE_BACKEND_SRCS) $(TEST_SRCS) -lgtest -lgtest_main -pthread -o $(TEST_DIR)/s21_test -lsubunit  -lgcov
	cd $(TEST_DIR)
	./$(TEST_DIR)/s21_test
	lcov --ignore-errors mismatch,gcov --no-external  -t "s21_test" -o $(BUILD_DIR)/s21_test.info -c -d .
	lcov --remove $(BUILD_DIR)/s21_test.info "controller*" -o $(BUILD_DIR)/s21_test_filtered.info
	genhtml -o report $(BUILD_DIR)/s21_test_filtered.info
	rm -rf tmp
	rm -rf $(BUILD_DIR)/*.info
	rm -rf $(BUILD_DIR)/*.gcda
	rm -rf $(BUILD_DIR)/*.gcno

#########################################
#----------- Installation --------------#
#########################################
.PHONY: install uninstall
install: clean console GUI
	 cd ../ && mkdir -p $(INSTALL_DIR)
	 cd ../ && cp -rf src/$(BUILD_DIR)/snakeConsole $(INSTALL_DIR)/
	 cd ../ && cp -rf src/$(BUILD_DIR)/snakeGUI $(INSTALL_DIR)/
	 cd ../ && cp -rf src/$(BUILD_DIR)/tetrisConsole $(INSTALL_DIR)/
	 cd ../ && cp -rf src/$(BUILD_DIR)/tetrisGUI $(INSTALL_DIR)/

uninstall:
	cd ../ && rm -rf $(INSTALL_DIR)

#########################################
#----- Create distributive archive -----#
#########################################
.PHONY: dist clean_dist
dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/brick_game ${DIST_DIR}/brick_game
	cd ../ && cp -rf src/gui ${DIST_DIR}/gui
	cd ../ && cp -rf src/*.cc ${DIST_DIR}/
	cd ../ && cp -rf src/Makefile ${DIST_DIR}/
	cd ../ && cp -rf src/Doxyfile ${DIST_DIR}/
	cd ../ && tar -czvf $(DIST_NAME) ${DIST_DIR}
	cd ../ && rm -rf ${DIST_DIR}

clean_dist:
	cd ../ && rm -rf $(DIST_NAME)
	
#########################################
#----------- Documentation -------------#
#########################################
dvi: 
	doxygen
	open docs/html/index.html

#########################################
#---------------- Checks ---------------#
#########################################
.PHONY: check
check:
	find . \( -regex '.*\.\(cc\)' \) ! -name '*test*' | xargs cppcheck --enable=all --suppress=missingIncludeSystem 
	find . \( -regex '.*\.\(c\)' \) ! -name '*test*' | xargs cppcheck --enable=all --suppress=missingIncludeSystem --inline-suppr
	find . \( -regex '.*\.\(cc\|hpp\|c\|h\)' \) ! -name '*test*' -exec clang-format -style='{BasedOnStyle: Google}' -n {} \;

.PHONY: check_fix
check_fix:
	find . \( -regex '.*\.\(cc\)' \) ! -name '*test*' | xargs cppcheck --enable=all --suppress=missingIncludeSystem
	find . \( -regex '.*\.\(cc\|hpp\|c\|h\)' \) ! -name '*test*' -exec clang-format -style='{BasedOnStyle: Google}' -i {} \;

.PHONY: valgrind
valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt 	./$(TEST_DIR)/test
	rm -rf ./*score.txt

clean:
	rm -rf $(BUILD_DIR)

clean_all: clean clean_dist uninstall
	rm -rf ./report

.PHONY: all clean
