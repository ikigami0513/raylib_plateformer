#include "Animation.hpp"

Animation::Animation(Texture2D texture, int width, int height, int current_row, float animationSpeed) {
    this->texture = texture;
    this->width = width;
    this->height = height;
    this->columns = this->texture.width / this->width;
    this->rows = this->texture.height / this->height;
    this->current_row = current_row;
    this->current_frame = 0.0;
    this->animationSpeed = animationSpeed;
}

Animation::Animation(std::string path, int width, int height, int current_row, float animationSpeed) {
    this->texture = LoadTexture(path.c_str());
    this->width = width;
    this->height = height;
    this->columns = this->texture.width / this->width;
    this->rows = this->texture.height / this->height;
    this->current_row = current_row;
    this->current_frame = 0.0f;
    this->animationSpeed = animationSpeed;
}

void Animation::Update(float deltaTime) {
    this->current_frame += this->animationSpeed * deltaTime;
    if (this->current_frame > this->columns) {
        this->current_frame = 0.0f;
    }
}

void Animation::Render(Vector2 position, Color tint) {
    Rectangle rect = {(int)this->current_frame * this->width, (this->current_row - 1) * this->height, this->width, this->height};
    DrawTextureRec(this->texture, rect, position, tint);
}
