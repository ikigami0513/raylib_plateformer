#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include "raylib.h"
#include "Player.hpp"

class Game {
    public:
        int screenWidth;
        int screenHeight;
        Player* player;
        std::vector<EnvItem> envItems;
        Camera2D camera;

        Game(int screenWidth, int screenHeight, std::string title);
        void GameLoop();

    private:
        void Update();
        void UpdateCamera();
        void Render();
};

#endif