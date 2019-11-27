#include "Game.h"

int main(int argc, char *args[]) {

  Game *game = new Game("Reversi");

  while (game->isRunning()) {
    game->handleEvents();
    game->render();
  }

  game->clean();
}
