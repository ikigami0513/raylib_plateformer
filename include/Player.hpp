#ifndef PLAYER_HPP 
#define PLAYER_HPP

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>

#include <EnvItem.hpp>

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

class Player {
    public:
        Vector2 position;
        float speed;
        bool canJump;

        Player(Vector2 position, float speed, bool canJump);
        void Update(std::vector<EnvItem>& envItems, float delta);
        void Draw();
};

#endif