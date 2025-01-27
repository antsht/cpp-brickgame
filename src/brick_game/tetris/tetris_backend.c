#include "tetris_backend.h"

void init_stats(game_stats_t *stats) {
  stats->level = 0;
  stats->score = 0;
  stats->high_score = 0;
  load_high_score(stats);
  // stats->next_advance_time_in_ms = timeInMilliseconds() + DEFAULT_DELAY_MS;
}

void init_board(board_t *board) {
  for (int i = 0; i < BOARD_ROWS; ++i) {
    for (int j = 0; j < BOARD_COLS; ++j) {
      board->board[i][j] = kColorBlack;
    }
  }

  board->tetramino_next = gen_next_tetramino();
}

bool check_lborder_collide(const tetramino_t *tetramino) {
  bool rc = false;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (tetramino->figure.sprite[tetramino->rotation][i][j] != 0 &&
          tetramino->col_pos + j < 0)
        rc = true;

  return rc;
}

bool check_rborder_collide(const tetramino_t *tetramino) {
  bool rc = false;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (tetramino->figure.sprite[tetramino->rotation][i][j] != 0 &&
          tetramino->col_pos + j > BOARD_COLS - 1)
        rc = true;

  return rc;
}

bool check_board_collide(const tetramino_t *tetramino, const board_t *board) {
  bool rc = false;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) {
      if (tetramino->figure.sprite[tetramino->rotation][i][j] != 0 &&
          tetramino->row_pos + i > BOARD_ROWS - 1)
        rc = true;
      if (tetramino->figure.sprite[tetramino->rotation][i][j] != 0 &&
          board->board[tetramino->row_pos + i][tetramino->col_pos + j] != 0)
        rc = true;
    }

  return rc;
}

void attach_tetramino(board_t *board) {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (board->tetramino_curr.figure
              .sprite[board->tetramino_curr.rotation][i][j] != 0)
        board->board[board->tetramino_curr.row_pos + i]
                    [board->tetramino_curr.col_pos + j] =
            board->tetramino_curr.figure.figure_color;
}

int find_full_rows(const board_t *board) {
  int full_row = -1;
  for (int row = 0; row < BOARD_ROWS; ++row) {
    int filled_count = 0;
    for (int col = 0; col < BOARD_COLS; ++col) {
      if (board->board[row][col] != 0) filled_count++;
    }
    if (filled_count == BOARD_COLS) {
      full_row = row;
      break;
    }
  }
  return full_row;
}

void shift_board(board_t *board, int row_number) {
  for (int row = row_number; row > 0; --row)
    for (int col = 0; col < BOARD_COLS; ++col)
      board->board[row][col] = board->board[row - 1][col];
}

void update_score(game_stats_t *stats, int rows_removed) {
  switch (rows_removed) {
    case 1:
      stats->score += SCORE_1;
      break;
    case 2:
      stats->score += SCORE_2;
      break;
    case 3:
      stats->score += SCORE_3;
      break;
    case 4:
      stats->score += SCORE_4;
      break;

    default:
      break;
  }

  if (stats->score > stats->high_score) {
    stats->high_score = stats->score;
    save_high_score(stats);
  }

  stats->level = stats->score / 600 + 1;
  if (stats->level > MAX_LEVEL) stats->level = MAX_LEVEL;
}

tetramino_t gen_next_tetramino() {
  figure_t figures[TETRAMINOS] = {
      {.sprite = {0}, .max_rotation = 0, .figure_color = 0}};
  /*
  ####
  */
  figures[0].figure_color = kColorCyan;
  figures[0].max_rotation = 1;
  figures[0].sprite[0][0][1] = 1;
  figures[0].sprite[0][1][1] = 1;
  figures[0].sprite[0][2][1] = 1;
  figures[0].sprite[0][3][1] = 1;

  figures[0].sprite[1][1][0] = 1;
  figures[0].sprite[1][1][1] = 1;
  figures[0].sprite[1][1][2] = 1;
  figures[0].sprite[1][1][3] = 1;
  /*
  ##
   ##
  */
  figures[1].figure_color = kColorRed;
  figures[1].max_rotation = 1;
  figures[1].sprite[0][0][0] = 1;
  figures[1].sprite[0][0][1] = 1;
  figures[1].sprite[0][1][1] = 1;
  figures[1].sprite[0][1][2] = 1;

  figures[1].sprite[1][0][1] = 1;
  figures[1].sprite[1][1][0] = 1;
  figures[1].sprite[1][1][1] = 1;
  figures[1].sprite[1][2][0] = 1;
  /*
   ##
  ##
  */
  figures[2].figure_color = kColorGreen;
  figures[2].max_rotation = 1;
  figures[2].sprite[0][0][1] = 1;
  figures[2].sprite[0][0][2] = 1;
  figures[2].sprite[0][1][0] = 1;
  figures[2].sprite[0][1][1] = 1;

  figures[2].sprite[1][0][0] = 1;
  figures[2].sprite[1][1][0] = 1;
  figures[2].sprite[1][1][1] = 1;
  figures[2].sprite[1][2][1] = 1;
  /*
   #
  ###
  */
  figures[3].figure_color = kColorMagenta;
  figures[3].max_rotation = 3;
  figures[3].sprite[0][0][1] = 1;
  figures[3].sprite[0][1][0] = 1;
  figures[3].sprite[0][1][1] = 1;
  figures[3].sprite[0][1][2] = 1;

  figures[3].sprite[1][0][1] = 1;
  figures[3].sprite[1][1][1] = 1;
  figures[3].sprite[1][1][2] = 1;
  figures[3].sprite[1][2][1] = 1;

  figures[3].sprite[2][1][0] = 1;
  figures[3].sprite[2][1][1] = 1;
  figures[3].sprite[2][1][2] = 1;
  figures[3].sprite[2][2][1] = 1;

  figures[3].sprite[3][0][1] = 1;
  figures[3].sprite[3][1][0] = 1;
  figures[3].sprite[3][1][1] = 1;
  figures[3].sprite[3][2][1] = 1;
  /*
   #
   #
   ##
  */
  figures[4].figure_color = kColorWhite;
  figures[4].max_rotation = 3;
  figures[4].sprite[0][0][1] = 1;
  figures[4].sprite[0][1][1] = 1;
  figures[4].sprite[0][2][1] = 1;
  figures[4].sprite[0][2][2] = 1;

  figures[4].sprite[1][1][0] = 1;
  figures[4].sprite[1][1][1] = 1;
  figures[4].sprite[1][1][2] = 1;
  figures[4].sprite[1][2][0] = 1;

  figures[4].sprite[2][0][0] = 1;
  figures[4].sprite[2][0][1] = 1;
  figures[4].sprite[2][1][1] = 1;
  figures[4].sprite[2][2][1] = 1;

  figures[4].sprite[3][0][2] = 1;
  figures[4].sprite[3][1][0] = 1;
  figures[4].sprite[3][1][1] = 1;
  figures[4].sprite[3][1][2] = 1;
  /*
   #
   #
  ##
  */
  figures[5].figure_color = kColorBlue;
  figures[5].max_rotation = 3;
  figures[5].sprite[0][0][1] = 1;
  figures[5].sprite[0][1][1] = 1;
  figures[5].sprite[0][2][0] = 1;
  figures[5].sprite[0][2][1] = 1;

  figures[5].sprite[1][0][0] = 1;
  figures[5].sprite[1][1][0] = 1;
  figures[5].sprite[1][1][1] = 1;
  figures[5].sprite[1][1][2] = 1;

  figures[5].sprite[2][0][0] = 1;
  figures[5].sprite[2][0][1] = 1;
  figures[5].sprite[2][1][0] = 1;
  figures[5].sprite[2][2][0] = 1;

  figures[5].sprite[3][1][0] = 1;
  figures[5].sprite[3][1][1] = 1;
  figures[5].sprite[3][1][2] = 1;
  figures[5].sprite[3][2][2] = 1;
  /*
  ##
  ##
  */
  figures[6].figure_color = kColorYellow;
  figures[6].max_rotation = 0;
  figures[6].sprite[0][0][0] = 1;
  figures[6].sprite[0][0][1] = 1;
  figures[6].sprite[0][1][0] = 1;
  figures[6].sprite[0][1][1] = 1;

  tetramino_t tetramino = {
      .row_pos = 0, .col_pos = 0, .figure = {}, .rotation = 0};
  srand(time(NULL));
  tetramino.figure = figures[rand() % TETRAMINOS];
  tetramino.col_pos = BOARD_COLS / 2 - 1;
  tetramino.row_pos = 0;
  tetramino.rotation = 0;
  return tetramino;
}

int save_high_score(const game_stats_t *stats) {
  int status = SUCCESS;
  FILE *file = fopen("./tetris_high_score.txt", "w+");
  if (file == NULL) {
    status = ERROR_T;
  } else {
    fprintf(file, "%d\n", stats->high_score);
    fclose(file);
  }
  return status;
}
int load_high_score(game_stats_t *stats) {
  int status = SUCCESS;
  FILE *file = fopen("./tetris_high_score.txt", "r");
  if (file == NULL) {
    status = ERROR_T;
  } else {
    fscanf(file, "%d", &stats->high_score);
    fclose(file);
  }
  return status;
}

uint64_t timeInMilliseconds(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((uint64_t)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}