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

#define EARLY 1
#define MIDDLE 2
#define LATE 3

class Board {
public:
  Board();

  Board(Board const &board);

  ~Board();

  int getMovesScore(int color);

  bool legalMove(int col, int row, int color);

  std::vector<Move> legalMoves(int color);

  void addMove(const Move &move, int color);

  void flipMove(const Move &move, int color);

  int getColor(int col, int row);

  void flipPieces(int col, int row, int color);

  int stage();

  static const int earlyVals[SIZE][SIZE];
  static const int middleVals[SIZE][SIZE];
  static const int lateVals[SIZE][SIZE];

  static const short neighbors[8][2];

  int totalMoves;
  Move *lastMove;

private:
  int moves[SIZE][SIZE]{};
};

#endif
