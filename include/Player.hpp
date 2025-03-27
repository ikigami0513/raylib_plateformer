#ifndef PLAYER_HPP 
#define PLAYER_HPP

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>
#include <map>

#include "EnvItem.hpp"
#include "Animation.hpp"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

enum State {
    IDLE,
    WALK,
};

enum Direction {
    LEFT,
    RIGHT
};

class Player {
    public:
        Vector2 position;
        float speed;
        bool canJump;
        State state;
        Direction direction;
        std::map<State, Animation*> animations;
        Animation* animation;

        Player(Vector2 position, float speed, bool canJump);
        void Update(std::vector<EnvItem>& envItems, float delta);
        void Draw();
};

#endif