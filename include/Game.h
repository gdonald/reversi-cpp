#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "Board.h"
#include "Move.h"

#define DEPTH 5

#define SCREEN_W  626
#define SCREEN_H 626
#define BOARD_HW  600
#define LABEL 24
#define DISC (BOARD_HW / SIZE)
#define RADIUS ((DISC / 2) - 2)

#define BTN_W 120
#define BTN_H 40
#define BTN_SPACE 20

#define FONT "res/font/LiberationSerif-Bold.ttf"

enum Buttons {
  BtnOptions, BtnQuit, BtnYes, BtnNo,
  BtnCount
};

enum ButtonStates {
  BtnUp = 0, BtnDown = 40, BtnOff = 80
};

enum Menus {
  MenuNone, MenuOptions, MenuGameOver
};

class Game {
public:
  explicit Game(const char *title);
  ~Game();

  bool isRunning();

  void handleEvents();

  void clean();

  void render();

  void drawGrid();

  void drawDiscs();

  void drawDisc(Sint16 col, Sint16 row, int color);

  void drawLegalMoves();

  void drawLegalMove(Sint16 col, Sint16 row);

  void drawLastMove();

  void drawOptionsMenu();

  void drawGameOverMenu();

  void drawMenu();

  void newGame();

  void handleClick(SDL_MouseButtonEvent *event);

  bool isPlayerTurn();

  void switchTurn();

  void aiTurn();

  bool insideRect(SDL_Rect rect, int x, int y);

  static int evaluate(Board *board, int color);

  static int otherColor(int color);

  static int minimax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer);

  static int colorScoreWeight(Board *board);

  static int mobilityScoreWeight(Board *board);

  Move getAiMove();

  void writeText(const char *text, int x, int y, TTF_Font *font);

  static void aiThread(Game *game);

  std::string letters[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
  std::string numbers[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};

private:
  bool running = false;
  SDL_Window *window{};
  SDL_Renderer *renderer{};
  SDL_Surface *bgSurface;
  SDL_Texture *bgTexture;
  Board *board{};

  int mouseX{};
  int mouseY{};

  int turn{};
  int currentMenu = MenuGameOver;

  SDL_Texture *btnTextures[BtnCount];
  SDL_Rect btnRects[BtnCount];

  SDL_Surface *btnOptionsSurface;
  SDL_Surface *btnQuitSurface;
  SDL_Surface *btnYesSurface;
  SDL_Surface *btnNoSurface;

  TTF_Font *font15;
  TTF_Font *font21;
};

#endif
