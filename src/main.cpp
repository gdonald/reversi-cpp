#include "Game.h"

int main(int argc, char *args[]) {
  std::srand(unsigned(std::time(nullptr)));

  Game *game = new Game("Reversi");

  while (game->isRunning()) {
    game->handleEvents();
    game->render();
  }

  game->clean();
}
