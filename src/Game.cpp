#include "Game.h"

Game::Game(const char *title) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);

  if (!window) {
    printf("Could not create window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetWindowMinimumSize(window, SCREEN_W, SCREEN_H);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("Count not get renderer! SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  bgSurface = SDL_LoadBMP("res/img/bg.bmp");
  if (bgSurface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/bg.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  SDL_FreeSurface(bgSurface);

  newGame();

  running = true;
}

bool Game::isRunning() {
  return running;
}

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&mouseX, &mouseY);
        handleClick(&event.button);
        break;
    }
  }
}

void Game::clean() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}

void Game::render() {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

  drawGrid();
  drawDiscs();

  SDL_RenderPresent(renderer);
  SDL_Delay(30);
}

void Game::drawDisc(Sint16 col, Sint16 row, char *color) {
  Sint16 x = (DISC * col) + RADIUS + 3;
  Sint16 y = (DISC * row) + RADIUS + 3;
  Uint8 c = !strcmp(color, dark) ? 0x00 : 0xFF;

  filledCircleRGBA(renderer, x, y, RADIUS, c, c, c, 0xff);
}

void Game::drawDiscs() {
  for (Sint16 r = 0; r < SIZE; r++)
    for (Sint16 c = 0; c < SIZE; c++)
      if (strcmp(board[r][c], empty) != 0)
        drawDisc(c, r, board[r][c]);
}

void Game::drawGrid() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

  for (int x = 0; x <= BOARD_HW; x += BOARD_HW / SIZE) {
    SDL_RenderDrawLine(renderer, x, 0, x, BOARD_HW);
    SDL_RenderDrawLine(renderer, x + 1, 0, x + 1, BOARD_HW);
  }

  for (int y = 0; y <= BOARD_HW; y += BOARD_HW / SIZE) {
    SDL_RenderDrawLine(renderer, 0, y, BOARD_HW, y);
    SDL_RenderDrawLine(renderer, 0, y + 1, BOARD_HW, y + 1);
  }

  filledCircleRGBA(renderer, DISC * 2, DISC * 2, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6, DISC * 2, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 2, DISC * 6, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6, DISC * 6, 4, 0x00, 0x00, 0x00, 0xFF);
}

void Game::newGame() {
  for (auto &r : board)
    for (auto &c : r)
      c = const_cast<char *>(empty);

  int m = SIZE / 2;
  board[m - 1][m - 1] = const_cast<char *>(light);
  board[m - 1][m] = const_cast<char *>(dark);
  board[m][m] = const_cast<char *>(light);
  board[m][m - 1] = const_cast<char *>(dark);

  turn = const_cast<char *>(dark);
  player = const_cast<char *>(dark);
  ai = const_cast<char *>(light);
}

void Game::handleClick(SDL_MouseButtonEvent *event) {
  if (!isPlayerTurn() || event->button != SDL_BUTTON_LEFT) {
    return;
  }

  int col = mouseX / DISC;
  int row = mouseY / DISC;

  if (legalMove(player, col, row)) {
    flipPieces(player, col, row);
    switchTurn();

    for (;;) {
      aiTurn();

      if (countLegalMoves(player) > 0)
        break;

      if (countLegalMoves(ai) == 0)
        break;
    }

    switchTurn();
  }
}

bool Game::isPlayerTurn() {
  return turn == player;
}

bool Game::legalMove(char *who, int col, int row) {
  int x, y;
  char *op = const_cast<char *>(!strcmp(who, dark) ? light : dark);

  if (strcmp(board[row][col], empty) != 0)
    return false;

  for (int n = 0; n < 8; n++) {
    y = row + neighbors[n][0];
    x = col + neighbors[n][1];

    if (y < 0 || x < 0 || y >= SIZE || x >= SIZE)
      continue;

    if (board[y][x] == op) {
      y += neighbors[n][0];
      x += neighbors[n][1];

      while (y >= 0 && x >= 0 && y < SIZE && x < SIZE) {
        if (board[y][x] == turn) {
          return true;
        }

        y += neighbors[n][0];
        x += neighbors[n][1];
      }
    }
  }

  return false;
}

void Game::switchTurn() {
  char *op = const_cast<char *>(!strcmp(player, dark) ? light : dark);

  if (!isPlayerTurn() && countLegalMoves(player) > 0) {
    turn = player;
  } else if (isPlayerTurn() && countLegalMoves(op) > 0) {
    turn = op;
  } else {
    // TODO: game over
  }
}

int Game::countLegalMoves(char *who) {
  int moves = 0;

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (legalMove(who, c, r))
        moves++;

  return moves;
}

int Game::countEmpty() {
  int total = 0;

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (!strcmp(board[r][c], empty))
        total++;

  return total;
}

void Game::aiTurn() {
  if (isPlayerTurn())
    return;

  char *color = const_cast<char *>(!strcmp(player, dark) ? light : dark);

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (legalMove(color, c, r)) {
        flipPieces(color, c, r);
        return;
      }
}

void Game::flipPieces(char *color, int col, int row) {
  int x, y, fx, fy;
  char *op = const_cast<char *>(!strcmp(color, dark) ? light : dark);

  board[row][col] = color;

  for (int n = 0; n < 8; n++) {
    y = row + neighbors[n][0];
    x = col + neighbors[n][1];

    if (y < 0 || x < 0 || y >= SIZE || x >= SIZE)
      continue;

    if (board[y][x] == op) {
      y += neighbors[n][0];
      x += neighbors[n][1];

      while (y >= 0 && x >= 0 && y < SIZE && x < SIZE) {
        if (board[y][x] == color) {
          fy = row;
          fx = col;

          for (;;) {
            fy += neighbors[n][0];
            fx += neighbors[n][1];
            board[fy][fx] = color;

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
