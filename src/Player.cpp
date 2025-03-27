#include "Player.hpp"

Player::Player(Vector2 position, float speed, bool canJump) {
    this->position = position;
    this->speed = speed;
    this->canJump = canJump;

    this->animations["idle"] = new Animation(
        "resources/character_templates/idle/player_idle_48_48.png",
        48, 48,
        1, 12
    );
    this->animation = this->animations["idle"];
}

void Player::Update(std::vector<EnvItem>& envItems, float deltaTime) {
    if (IsKeyDown(KEY_A)) this->position.x -= PLAYER_HOR_SPD * deltaTime;
	if (IsKeyDown(KEY_D)) this->position.x += PLAYER_HOR_SPD * deltaTime;
	if (IsKeyDown(KEY_SPACE) && this->canJump) {
		this->speed = -PLAYER_JUMP_SPD;
		this->canJump = false;
	}

	bool hitObstacle = false;
	for (auto& ei: envItems) {
		if (
			ei.blocking &&
			ei.rect.x <= this->position.x &&
			ei.rect.x + ei.rect.width >= this->position.x &&
			ei.rect.y >= this->position.y &&
			ei.rect.y <= this->position.y + this->speed * deltaTime
		) {
			hitObstacle = true;
			this->speed = 0.0f;
			this->position.y = ei.rect.y;
			break;
		}
	}

	if (!hitObstacle) {
		this->position.y += this->speed * deltaTime;
		this->speed += G * deltaTime;
		this->canJump = false;
	}
	else {
		this->canJump = true;
	}

    this->animation->Update(deltaTime);
}

void Player::Draw() {
    // Rectangle rect = { this->position.x - 20, this->position.y - 40, 40.0f, 40.0f };
	// DrawRectangleRec(rect, RED);

	// DrawCircleV(this->position, 5.0f, GOLD);
    this->animation->Render({ this->position.x - 20, this->position.y - 40});
}
