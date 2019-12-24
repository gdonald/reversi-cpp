#include "Board.h"

const int Board::earlyVals[SIZE][SIZE] = {{2, 0, 1, 1, 1, 1, 0, 2},
                                          {0, 0, 5, 5, 5, 5, 0, 0},
                                          {1, 5, 5, 5, 5, 5, 5, 1},
                                          {1, 5, 5, 5, 5, 5, 5, 1},
                                          {1, 5, 5, 5, 5, 5, 5, 1},
                                          {1, 5, 5, 5, 5, 5, 5, 1},
                                          {0, 0, 5, 5, 5, 5, 0, 0},
                                          {2, 0, 1, 1, 1, 1, 0, 2}};

const int Board::middleVals[SIZE][SIZE] = {{10, 0, 2, 2, 2, 2, 0, 10},
                                           { 0, 0, 5, 5, 5, 5, 0,  0},
                                           { 2, 5, 5, 5, 5, 5, 5,  2},
                                           { 2, 5, 5, 5, 5, 5, 5,  2},
                                           { 2, 5, 5, 5, 5, 5, 5,  2},
                                           { 2, 5, 5, 5, 5, 5, 5,  2},
                                           { 0, 0, 5, 5, 5, 5, 0,  0},
                                           {10, 0, 2, 2, 2, 2, 0, 10}};

const int Board::lateVals[SIZE][SIZE] = {{100, 5, 8, 8, 8, 8, 5, 100},
                                         {  5, 0, 9, 9, 9, 9, 0,   5},
                                         {  8, 9, 9, 9, 9, 9, 9,   8},
                                         {  8, 9, 9, 9, 9, 9, 9,   8},
                                         {  8, 9, 9, 9, 9, 9, 9,   8},
                                         {  8, 9, 9, 9, 9, 9, 9,   8},
                                         {  5, 0, 9, 9, 9, 9, 0,   5},
                                         {100, 5, 8, 8, 8, 8, 5, 100}};

const short Board::neighbors[8][2] = {{-1, -1},
                                      {-1, 0},
                                      {-1, 1},
                                      {0,  1},
                                      {1,  1},
                                      {1,  0},
                                      {1,  -1},
                                      {0,  -1}};

Board::~Board() {
  delete lastMove;
}

Board::Board() {
  for (int row = 0; row < SIZE; row++)
    for (int col = 0; col < SIZE; col++)
      moves[row][col] = EMPTY;
  totalMoves = 0;
  lastMove = new Move(-1, -1);

  int m = SIZE / 2;
  addMove(Move(m - 1, m - 1), LIGHT);
  addMove(Move(m - 1, m), DARK);
  addMove(Move(m, m), LIGHT);
  addMove(Move(m, m - 1), DARK);
}

Board::Board(Board const &board) {
  for (int row = 0; row < SIZE; row++)
    for (int col = 0; col < SIZE; col++)
      moves[row][col] = board.moves[row][col];
  totalMoves = board.totalMoves;
  lastMove = new Move(board.lastMove);
}

void Board::flipMove(const Move &move, int color) {
  if (moves[move.row][move.col] == EMPTY) {
    std::cout << "Cannot flip empty: col: " << move.col << ", row: " << move.row << std::endl;
    throw;
  }

  moves[move.row][move.col] = color;
}

void Board::addMove(const Move &move, int color) {
  if (moves[move.row][move.col] != EMPTY) {
    std::cout << "Cannot add move to occupied: col: " << move.col << ", row: " << move.row << std::endl;
    throw;
  }

  moves[move.row][move.col] = color;
  totalMoves++;
  lastMove->col = move.col;
  lastMove->row = move.row;
}

void Board::flipPieces(int col, int row, int color) {
  int x, y, fx, fy;
  int op = color == DARK ? LIGHT : DARK;

  addMove(Move(col, row), color);

  for (int n = 0; n < 8; n++) {
    y = row + neighbors[n][0];
    x = col + neighbors[n][1];

    if (y < 0 || x < 0 || y >= SIZE || x >= SIZE)
      continue;

    if (getColor(x, y) == op) {
      y += neighbors[n][0];
      x += neighbors[n][1];

      while (y >= 0 && x >= 0 && y < SIZE && x < SIZE) {
        if (moves[y][x] == EMPTY) {
          break;
        }

        if (getColor(x, y) == color) {
          fy = row;
          fx = col;

          for (;;) {
            fy += neighbors[n][0];
            fx += neighbors[n][1];
            flipMove(Move(fx, fy), color);

            if (fy == y && fx == x)
              break;
          }

          break;
        }

        y += neighbors[n][0];
        x += neighbors[n][1];
      }
    }
  }
}

int Board::getMovesScore(int color) {
  int total = 0;

  for (int row = 0; row < SIZE; row++)
    for (int col = 0; col < SIZE; col++)
      if (moves[row][col] == color) {
        switch(stage()) {
          case EARLY:
            total += earlyVals[row][col];
            break;
          case MIDDLE:
            total += middleVals[row][col];
            break;
          case LATE:
            total += lateVals[row][col];
            break;
        }
      }

  return total;
}

bool Board::legalMove(int col, int row, int color) {
  int x, y;
  int op = color == DARK ? LIGHT : DARK;

  if (moves[row][col] != EMPTY)
    return false;

  for (int n = 0; n < 8; n++) {
    y = row + neighbors[n][0];
    x = col + neighbors[n][1];

    if (y < 0 || x < 0 || y >= SIZE || x >= SIZE)
      continue;

    if (moves[y][x] == op) {
      y += neighbors[n][0];
      x += neighbors[n][1];

      while (y >= 0 && x >= 0 && y < SIZE && x < SIZE) {
        if (moves[y][x] == EMPTY) {
          break;
        }

        if (moves[y][x] == color) {
          return true;
        }

        y += neighbors[n][0];
        x += neighbors[n][1];
      }
    }
  }

  return false;
}

std::vector<Move> Board::legalMoves(int color) {
  std::vector<Move> v;

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (legalMove(c, r, color))
        v.emplace_back(c, r);

  return v;
}

int Board::getColor(int col, int row) {
  return moves[row][col];
}

int Board::stage() {
  if (totalMoves <= 20) { return EARLY; }
  if (totalMoves <= 40) { return MIDDLE; }
  return LATE;
}
