#ifndef MOVE_H
#define MOVE_H

class Move {
public:
  Move(int col, int row);
  Move(const Move &move);

  int col{};
  int row{};
};

#endif
