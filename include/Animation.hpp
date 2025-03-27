#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>
#include "raylib.h"

class Animation {
    public:
        Animation(Texture2D texture, int width, int height, int current_row, float animationSpeed);
        Animation(std::string texture, int width, int height, int current_row, float animationSpeed);
        void Update(float deltaTime);
        void Render(Vector2 position, Color tint = WHITE, bool flipX = false);
        void Reset();

    private:
        Texture2D texture;
        int width, height;
        int columns, rows;
        int current_row;
        float current_frame;
        float animationSpeed;
};

#endif