
#include "Game.h"

Game::~Game() {
  TTF_CloseFont(font15);
  TTF_CloseFont(font21);
  TTF_Quit();

  SDL_DestroyTexture(btnTextures[BtnOptions]);
  SDL_DestroyTexture(btnTextures[BtnQuit]);
  SDL_DestroyTexture(btnTextures[BtnYes]);
  SDL_DestroyTexture(btnTextures[BtnNo]);

  SDL_DestroyTexture(bgTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

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

  btnOptionsSurface = SDL_LoadBMP("res/img/btn_options.bmp");
  btnQuitSurface = SDL_LoadBMP("res/img/btn_quit.bmp");
  btnYesSurface = SDL_LoadBMP("res/img/btn_yes.bmp");
  btnNoSurface = SDL_LoadBMP("res/img/btn_no.bmp");

  if (btnOptionsSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_options.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (btnQuitSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_quit.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (btnYesSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_yes.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (btnNoSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_no.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  btnTextures[BtnOptions] = SDL_CreateTextureFromSurface(renderer, btnOptionsSurface);
  btnTextures[BtnQuit] = SDL_CreateTextureFromSurface(renderer, btnQuitSurface);
  btnTextures[BtnYes] = SDL_CreateTextureFromSurface(renderer, btnYesSurface);
  btnTextures[BtnNo] = SDL_CreateTextureFromSurface(renderer, btnNoSurface);

  SDL_FreeSurface(btnOptionsSurface);
  SDL_FreeSurface(btnQuitSurface);
  SDL_FreeSurface(btnYesSurface);
  SDL_FreeSurface(btnNoSurface);

  if (TTF_Init() == -1) {
    printf("TTF_Init failed: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  font15 = TTF_OpenFont(FONT, 15);
  if (font15 == nullptr) {
    printf("Failed to load font15! Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  font21 = TTF_OpenFont(FONT, 21);
  if (font21 == nullptr) {
    printf("Failed to load font21! Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  newGame();

  running = true;
}

bool Game::isRunning() {
  return running;
}

void Game::handleEvents() {
  SDL_Event event;

  if (SDL_WaitEvent(&event)) {
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
  drawLastMove();
  drawDiscs();
  drawLegalMoves();
  drawMenu();

  SDL_RenderPresent(renderer);
  SDL_Delay(30);
}

bool Game::insideRect(SDL_Rect rect, int x, int y) {
  return x > rect.x &&
         x < rect.x + rect.w &&
         y > rect.y &&
         y < rect.y + rect.h;
}

void Game::drawMenu() {
  switch (currentMenu) {
    case MenuOptions:
      drawOptionsMenu();
      break;
    case MenuGameOver:
      drawGameOverMenu();
      break;
  }
}

void Game::drawOptionsMenu() {
  // TODO
}

void Game::drawGameOverMenu() {

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xcc);
  SDL_Rect rect;
  rect.x = SCREEN_W / 2 - 160;
  rect.y = SCREEN_W / 2 - 85;
  rect.h = 170;
  rect.w = 320;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
  SDL_RenderDrawRect(renderer, &rect);

  std::ostringstream result;
  result << "Game Over: ";

  int darkMovesScore = board->getMovesScore(DARK);
  int lightMovesScore = board->getMovesScore(LIGHT);

  if (darkMovesScore > lightMovesScore) {
    result << "Dark Wins";
  } else if(lightMovesScore > darkMovesScore) {
    result << "Light Wins";
  } else {
    result << "Tie";
  }

  writeText(result.str().c_str(), 184, 250, font21);
  writeText("Play Again?", 184, 285, font21);

  SDL_Rect clip[BtnCount];
  clip[BtnNo].x = 0;
  clip[BtnNo].y = BtnUp;
  clip[BtnNo].w = BTN_W;
  clip[BtnNo].h = BTN_H;

  clip[BtnYes].x = 0;
  clip[BtnYes].y = BtnUp;
  clip[BtnYes].w = BTN_W;
  clip[BtnYes].h = BTN_H;

  btnRects[BtnNo].x = (SCREEN_W / 2) - BTN_W - (BTN_SPACE / 2);
  btnRects[BtnNo].y = (int) 330;
  btnRects[BtnNo].w = BTN_W;
  btnRects[BtnNo].h = BTN_H;

  btnRects[BtnYes].x = (SCREEN_W / 2) + (BTN_SPACE / 2);
  btnRects[BtnYes].y = (int) 330;
  btnRects[BtnYes].w = BTN_W;
  btnRects[BtnYes].h = BTN_H;

  SDL_RenderCopy(renderer, btnTextures[BtnNo], &clip[BtnNo], &btnRects[BtnNo]);
  SDL_RenderCopy(renderer, btnTextures[BtnYes], &clip[BtnYes], &btnRects[BtnYes]);
}

void Game::drawDisc(Sint16 col, Sint16 row, int color) {
  Sint16 x = (DISC * col) + RADIUS + 3;
  Sint16 y = (DISC * row) + RADIUS + 3;
  Uint8 c = color == DARK ? 0x00 : 0xFF;

  filledCircleRGBA(renderer, LABEL + x, LABEL + y, RADIUS, c, c, c, 0xff);
}

void Game::drawDiscs() {
  for (Sint16 r = 0; r < SIZE; r++) {
    for (Sint16 c = 0; c < SIZE; c++) {
      int color = board->getColor(c, r);
      if (color != EMPTY) { drawDisc(c, r, color); }
    }
  }
}

void Game::drawLegalMoves() {
  if (currentMenu != MenuNone) { return; }
  auto moves = board->legalMoves(turn);
  for (auto &move: moves) { drawLegalMove(move.col, move.row); }
}

void Game::drawLegalMove(Sint16 col, Sint16 row) {
  Sint16 x = (DISC * col) + RADIUS + 3;
  Sint16 y = (DISC * row) + RADIUS + 3;

  if (isPlayerTurn()) {
    filledCircleRGBA(renderer, LABEL + x + 1, LABEL + y + 1, 5, 0x00, 0x00, 0x00, 0xaa);
    filledCircleRGBA(renderer, LABEL + x + 1, LABEL + y + 1, 2, 0xFF, 0xFF, 0xFF, 0xaa);
  } else {
    filledCircleRGBA(renderer, LABEL + x + 1, LABEL + y + 1, 5, 0xee, 0xee, 0xee, 0xaa);
    filledCircleRGBA(renderer, LABEL + x + 1, LABEL + y + 1, 2, 0xFF, 0x00, 0x00, 0xdd);
  }
}

void Game::drawLastMove() {
  if (board->totalMoves <= 4) { return; }

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
  SDL_Rect rect;
  rect.x = LABEL + (DISC * board->lastMove->col);
  rect.y = LABEL + (DISC * board->lastMove->row);
  rect.h = DISC + 1;
  rect.w = DISC + 1;
  SDL_RenderDrawRect(renderer, &rect);
}

void Game::drawGrid() {
  int lb = LABEL + BOARD_HW;

  SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xff);

  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.h = LABEL;
  rect.w = LABEL + BOARD_HW;
  SDL_RenderFillRect(renderer, &rect);

  rect.x = 0;
  rect.y = 0;
  rect.h = LABEL + BOARD_HW;
  rect.w = LABEL;
  SDL_RenderFillRect(renderer, &rect);

  for (int x = 0; x < 8; x++)
    writeText(letters[x].c_str(), x * DISC + LABEL + RADIUS + 1, 2, font15);

  for (int x = 0; x < 8; x++)
    writeText(numbers[x].c_str(), 8, x * DISC + LABEL + RADIUS - 7, font15);

  SDL_SetRenderDrawColor(renderer, 0x00, 0xaa, 0x00, 0xaa);

  // vertical lines
  SDL_RenderDrawLine(renderer, 0, 0, 0, lb);
  for (int x = LABEL; x <= lb; x += BOARD_HW / SIZE) {
    SDL_RenderDrawLine(renderer, x, 0, x, lb);
  }

  // horizontal lines
  SDL_RenderDrawLine(renderer, 0, 0, lb, 0);
  for (int y = LABEL; y <= lb; y += BOARD_HW / SIZE) {
    SDL_RenderDrawLine(renderer, 0, y, lb, y);
  }

  filledCircleRGBA(renderer, LABEL + DISC * 2, LABEL + DISC * 2, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6, LABEL + DISC * 2, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 2, LABEL + DISC * 6, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6, LABEL + DISC * 6, 4, 0x00, 0xaa, 0x00, 0xaa);

  filledCircleRGBA(renderer, LABEL + DISC * 2 + 1, LABEL + DISC * 2, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6 + 1, LABEL + DISC * 2, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 2 + 1, LABEL + DISC * 6, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6 + 1, LABEL + DISC * 6, 4, 0x00, 0xaa, 0x00, 0xaa);

  filledCircleRGBA(renderer, LABEL + DISC * 2, LABEL + DISC * 2 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6, LABEL + DISC * 2 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 2, LABEL + DISC * 6 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6, LABEL + DISC * 6 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);

  filledCircleRGBA(renderer, LABEL + DISC * 2 + 1, LABEL + DISC * 2 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6 + 1, LABEL + DISC * 2 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 2 + 1, LABEL + DISC * 6 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
  filledCircleRGBA(renderer, LABEL + DISC * 6 + 1, LABEL + DISC * 6 + 1, 4, 0x00, 0xaa, 0x00, 0xaa);
}

void Game::newGame() {
  board = new Board();
  turn = DARK;
}

void Game::handleClick(SDL_MouseButtonEvent *event) {
  switch (currentMenu) {
    case MenuGameOver:
      if (insideRect(btnRects[BtnNo], mouseX, mouseY)) {
        currentMenu = MenuNone;
        render();
        return;
      }
      if (insideRect(btnRects[BtnYes], mouseX, mouseY)) {
        currentMenu = MenuNone;
        newGame();
        render();
        return;
      }
      break;
  }

  if (!isPlayerTurn() || event->button != SDL_BUTTON_LEFT) { return; }

  int col = (mouseX - LABEL) / DISC;
  int row = (mouseY - LABEL) / DISC;

  if (board->legalMove(col, row, DARK)) {
    board->flipPieces(col, row, DARK);
    switchTurn();
    render();

    std::thread t{aiThread, this};
    t.join();

    render();
  }
}

void Game::aiThread(Game *game) {
  for (;;) {
    game->aiTurn();

    if (!game->board->legalMoves(DARK).empty())
      break;

    if (game->board->legalMoves(LIGHT).empty())
      break;
  }

  game->switchTurn();
}

bool Game::isPlayerTurn() {
  return turn == DARK;
}

void Game::switchTurn() {
  bool darkCanGo = !board->legalMoves(DARK).empty();
  bool lightCanGo = !board->legalMoves(LIGHT).empty();

  if (!isPlayerTurn() && darkCanGo) {
    turn = DARK;
  } else if (isPlayerTurn() && lightCanGo) {
    turn = LIGHT;
  } else if(!darkCanGo && !lightCanGo) {
    currentMenu = MenuGameOver;
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

  for (auto &move : moves) {
    auto childBoard = Board(*board);
    childBoard.addMove(move, LIGHT);
    eval = minimax(&childBoard, DEPTH, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
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

int Game::colorScoreWeight(Board *board) {
  if (board->totalMoves == 0) { return 1; }
  return board->totalMoves * 500;
}

int Game::mobilityScoreWeight(Board *board) {
  if (board->totalMoves == 0) { return 1; }
  return 100000 / board->totalMoves;
}

void Game::writeText(const char *text, const int x, const int y, TTF_Font *font) {
  SDL_Color color = {255, 255, 255, 0};
  int w, h;

  SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_SizeText(font, text, &w, &h);
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

  SDL_RenderCopy(renderer, texture, nullptr, &rect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}
