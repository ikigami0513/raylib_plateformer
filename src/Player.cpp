#include "Player.hpp"

Player::Player(Vector2 position, float speed, bool canJump) {
    this->position = position;
    this->speed = speed;
    this->canJump = canJump;

    this->animations[IDLE] = new Animation("resources/character_templates/idle/player_idle_48_48.png", 48, 48, 1, 12);
    this->animations[WALK] = new Animation("resources/character_templates/walk/player_walk_48_48.png", 48, 48, 1, 12);
    this->state = IDLE;
    this->direction = RIGHT;
    this->animation = this->animations[this->state];
}

void Player::Update(std::vector<EnvItem>& envItems, float deltaTime) {
    State newState = this->state;
    if (IsKeyDown(KEY_A)) {
        this->position.x -= PLAYER_HOR_SPD * deltaTime;
        newState = WALK;
        this->direction = LEFT;
    }
	else if (IsKeyDown(KEY_D)) {
        this->position.x += PLAYER_HOR_SPD * deltaTime;
        newState = WALK;
        this->direction = RIGHT;
    }
    else {
        newState = IDLE;
    }
    
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

    if (newState != this->state) {
        this->state = newState;
        this->animation = this->animations[this->state];
        this->animation->Reset();
    }
    this->animation->Update(deltaTime);
}

void Player::Draw() {
    if (this->direction == LEFT) {
        this->animation->Render({ this->position.x - 20, this->position.y - 40}, WHITE, true);
    }
    else {
        this->animation->Render({ this->position.x - 20, this->position.y - 40});
    }
}
