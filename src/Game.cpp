
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
  if (bgSurface == nullptr) {
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
  SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);

  drawGrid();
  drawDiscs();
  drawLegalMoves();

  SDL_RenderPresent(renderer);
  SDL_Delay(30);
}

void Game::drawDisc(Sint16 col, Sint16 row, int color) {
  Sint16 x = (DISC * col) + RADIUS + 3;
  Sint16 y = (DISC * row) + RADIUS + 3;
  Uint8 c = color == DARK ? 0x00 : 0xFF;

  filledCircleRGBA(renderer, x, y, RADIUS, c, c, c, 0xff);
}

void Game::drawDiscs() {
  for (Sint16 r = 0; r < SIZE; r++) {
    for (Sint16 c = 0; c < SIZE; c++) {
      int color = board->getColor(c, r);
      if (color != EMPTY) {
        drawDisc(c, r, color);
      }
    }
  }
}

void Game::drawLegalMoves() {
  auto moves = board->legalMoves(turn);

  for(auto & move: moves) {
    drawLegalMove(move.col, move.row);
  }
}

void Game::drawLegalMove(Sint16 col, Sint16 row) {
  Sint16 x = (DISC * col) + RADIUS + 3;
  Sint16 y = (DISC * row) + RADIUS + 3;

  filledCircleRGBA(renderer, x + 1, y + 1, 3, 0x00, 0xff, 0x00, 0xff);
  filledCircleRGBA(renderer, x + 1, y + 1, 2, 0xFF, 0xFF, 0xFF, 0x88);
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

  filledCircleRGBA(renderer, DISC * 2 + 1, DISC * 2, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6 + 1, DISC * 2, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 2 + 1, DISC * 6, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6 + 1, DISC * 6, 4, 0x00, 0x00, 0x00, 0xFF);

  filledCircleRGBA(renderer, DISC * 2, DISC * 2 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6, DISC * 2 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 2, DISC * 6 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6, DISC * 6 + 1, 4, 0x00, 0x00, 0x00, 0xFF);

  filledCircleRGBA(renderer, DISC * 2 + 1, DISC * 2 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6 + 1, DISC * 2 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 2 + 1, DISC * 6 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
  filledCircleRGBA(renderer, DISC * 6 + 1, DISC * 6 + 1, 4, 0x00, 0x00, 0x00, 0xFF);
}

void Game::newGame() {
  board = new Board();
  turn = DARK;
}

void Game::handleClick(SDL_MouseButtonEvent *event) {
  if (!isPlayerTurn() || event->button != SDL_BUTTON_LEFT) {
    return;
  }

  int col = mouseX / DISC;
  int row = mouseY / DISC;

  if (board->legalMove(col, row, DARK)) {
    board->flipPieces(col, row, DARK);
    switchTurn();

    for (;;) {
      aiTurn();

      if (!board->legalMoves(DARK).empty())
        break;

      if (board->legalMoves(LIGHT).empty())
        break;
    }

    switchTurn();
  }
}

bool Game::isPlayerTurn() {
  return turn == DARK;
}

void Game::switchTurn() {
  if (!isPlayerTurn() && !board->legalMoves(DARK).empty()) {
    turn = DARK;
  } else if (isPlayerTurn() && !board->legalMoves(LIGHT).empty()) {
    turn = LIGHT;
  } else {
    // TODO: game over
  }
}

void Game::aiTurn() {
  if (isPlayerTurn())
    return;

  Move m = getAiMove();

  if (m.col > -1 && m.row > -1) {
    board->flipPieces(m.col, m.row, LIGHT);
  }
}

int Game::evaluate(Board *board, int color) {
  int colorScore = board->getMovesScore(color);
  int otherColorScore = board->getMovesScore(otherColor(color));
  int finalColorScore = colorScore - otherColorScore;

  int mobilityScore = board->legalMoves(color).size();
  int otherMobilityScore = board->legalMoves(otherColor(color)).size();
  int finalMobilityScore = mobilityScore - otherMobilityScore;

  return (colorScoreWeight(board) * finalColorScore) + (mobilityScoreWeight(board) * finalMobilityScore);
}

int Game::otherColor(int color) {
  return color == DARK ? LIGHT : DARK;
}

Move Game::getAiMove() {
  auto moves = board->legalMoves(LIGHT);
  int eval;
  int maxEval = std::numeric_limits<int>::min();
  Move bestMove = Move(-1, -1);

  for(auto &move : moves) {
    auto childBoard = Board(*board);
    childBoard.addMove(move, LIGHT);
    eval = minimax(&childBoard, 5, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
    if (eval > maxEval) {
      maxEval = eval;
      bestMove = move;
    }
  }

  return bestMove;
}

int Game::minimax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer) {

  if (depth == 0 || board->legalMoves(LIGHT).empty()) {
    return evaluate(board, LIGHT);
  }

  int eval;

  if (maximizingPlayer) {
    int maxEval = std::numeric_limits<int>::min();

    for (auto &move : board->legalMoves(LIGHT)) {
      auto childBoard = Board(*board);
      childBoard.addMove(move, LIGHT);
      eval = minimax(&childBoard, depth - 1, alpha, beta, false);
      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);
      if (beta <= alpha)
        break;
    }

    return maxEval;

  } else {
    int minEval = std::numeric_limits<int>::max();

    for (auto &move : board->legalMoves(DARK)) {
      auto childBoard = Board(*board);
      childBoard.addMove(move, DARK);
      eval = minimax(&childBoard, depth - 1, alpha, beta, true);
      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);
      if (beta <= alpha)
        break;
    }

    return minEval;
  }
}

bool Game::gameOver() {
  return board->legalMoves(DARK).empty() && board->legalMoves(LIGHT).empty();
}

int Game::colorScoreWeight(Board *board) {
  int totalMoves = board->totalMoves();
  if (totalMoves == 0) { return 1; }
  return 540 / totalMoves;
}

int Game::mobilityScoreWeight(Board *board) {
  int totalMoves = board->totalMoves();
  if (totalMoves == 0) { return 1; }
  return totalMoves / 2;
}
