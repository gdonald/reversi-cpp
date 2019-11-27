#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "Board.h"

#define SCREEN_W  800
#define SCREEN_H  602
#define BOARD_HW  600
#define DISC      (BOARD_HW / SIZE)
#define RADIUS    ((DISC / 2) - 2)

#define MOBIL_MOD  5.0f // lower for more mobility points
#define BOARD_MOD 30.0f // higher for more board points

const short neighbors[8][2] = { { -1, -1 },
                                { -1,  0 },
                                { -1,  1 },
                                {  0,  1 },
                                {  1,  1 },
                                {  1,  0 },
                                {  1, -1 },
                                {  0, -1 } };

const int vals[SIZE][SIZE] = { { 30, 5, 5, 5, 5, 5, 5, 30 },
                               {  5, 0, 1, 1, 1, 1, 0,  5 },
                               {  5, 1, 1, 1, 1, 1, 1,  5 },
                               {  5, 1, 1, 1, 1, 1, 1,  5 },
                               {  5, 1, 1, 1, 1, 1, 1,  5 },
                               {  5, 1, 1, 1, 1, 1, 1,  5 },
                               {  5, 0, 1, 1, 1, 1, 0,  5 },
                               { 30, 5, 5, 5, 5, 5, 5, 30 } };

struct edge_nodes
{
  int max;
  int count;
  struct bnode **nodes;
};

class Game {
public:
  explicit Game(const char *title);

  bool isRunning();

  void handleEvents();

  void clean();

  void render();

  void drawGrid();

  void drawDiscs();

  void drawDisc(Sint16 col, Sint16 row, char *color);

  void newGame();

  void handleClick(SDL_MouseButtonEvent *event);

  bool isPlayerTurn();

  bool legalMove(char *who, int col, int row);

  void switchTurn();

  int countLegalMoves(char *who);

  int countEmpty();

  void aiTurn();

  void flipPieces(char *color, int col, int row);

private:
  bool running = false;
  SDL_Window *window{};
  SDL_Renderer *renderer{};
  SDL_Surface *bgSurface;
  SDL_Texture *bgTexture;
  Board *board{};

  char const *empty = "-";
  char const *dark = "x";
  char const *light = "o";

  int mouseX{};
  int mouseY{};

  char *turn{};
  char *player{};
  char *ai{};

  struct edge_nodes *edges{};
  struct bnode *root{};
};

#endif
