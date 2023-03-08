#pragma once

#include "game_layer.h"

class Game : public Toy2D::Application {
public:
    Game(const std::filesystem::path& game_root_path) :
        Toy2D::Application(game_root_path) {
        pushLayer(new GameLayer());
    }

    ~Game() {
    }
};

Toy2D::Application* createApp() {
    return new Game(std::filesystem::path(TOY2D_XSTR(GAME_ROOT_DIR)));
}
