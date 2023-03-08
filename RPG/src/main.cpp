/*
    description: Code for a 2D RPG Game
    author: yichenwu11
*/

#include "game.h"

int main() {
    auto game = createApp();

    game->run();

    delete game;
    game = nullptr;
}