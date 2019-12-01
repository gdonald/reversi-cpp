#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Move.h"

#define SIZE 8

#define DARK -1
#define EMPTY 0
#define LIGHT 1

const int vals[SIZE][SIZE] = {{50, 6, 5, 5, 5, 5, 6, 50},
                              {6,  0, 3, 1, 1, 3, 0,  6},
                              {5,  3, 3, 1, 1, 3, 3,  5},
                              {5,  1, 1, 1, 1, 1, 1,  5},
                              {5,  1, 1, 1, 1, 1, 1,  5},
                              {5,  3, 3, 1, 1, 3, 3,  5},
                              {6,  0, 3, 1, 1, 3, 0,  6},
                              {50, 6, 5, 5, 5, 5, 6, 50}};

const short neighbors[8][2] = {{-1, -1},
                               {-1, 0},
                               {-1, 1},
                               {0,  1},
                               {1,  1},
                               {1,  0},
                               {1,  -1},
                               {0,  -1}};

class Board {
public:
  Board();

  Board(Board const &board);

  int getMovesScore(int color);

  bool legalMove(int col, int row, int color);

  std::vector<Move> legalMoves(int color);

  void addMove(const Move &move, int color);

  void flipMove(const Move &move, int color);

  int getColor(int col, int row);

  void flipPieces(int col, int row, int color);

  int totalMoves();

  static const int vals[SIZE][SIZE];
  static const short neighbors[8][2];

private:
  int moves[SIZE][SIZE]{};
};

#endif