#include "Move.h"

Move::Move(int col, int row) : col(col), row(row) {}

Move::Move(const Move &move) = default;
