#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

struct Player {
	Vector2 position;
	float speed;
	bool canJump;
};

class Player {
	public:
		Vector2 position;
		float speed;
		bool canJump;

		Player(Vector2 position, float speed, bool canJump) {
			this->position = position;
			this->speed = speed;
			this->canJump = canJump;
		}

		void Update(std::vector<EnvItem>& envItems, float delta) {
			if (IsKeyDown(KEY_LEFT)) this->position.x -= PLAYER_HOR_SPD * delta;
			if (IsKeyDown(KEY_RIGHT)) this->position.x += PLAYER_HOR_SPD * delta;
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
					ei.rect.y <= this->position.y + this->speed * delta
				) {
					hitObstacle = true;
					this->speed = 0.0f;
					this->position.y = ei.rect.y;
					break;
				}
			}

			if (!hitObstacle) {
				this->position.y += this->speed * delta;
				this->speed += G * delta;
				this->canJump = false;
			}
			else {
				this->canJump = true;
			}
		}

		void Draw() {
			Rectangle rect = { this->position.x - 20, this->position.y - 40, 40.0f, 40.0f };
			DrawRectangleRec(rect, RED);

			DrawCircleV(this->position, 5.0f, GOLD);
		}
};

struct EnvItem {
	Rectangle rect;
	int blocking;
	Color color;
};

// Module functions declaration
void UpdateCameraCenter(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);
void UpdateCameraCenterInsideMap(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "2d camera");

	Player* player = new Player({ 400, 280}, 0, false);
	std::vector<EnvItem> envItems = {
		{{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
        {{ 0, 400, 1000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
	};

	Camera2D camera = { player->position, { screenWidth / 2.0f, screenHeight / 2.0f }, 0.0f, 1.0f };

	// Store pointers to the multiple update camera functions
	using CameraUpdaterFunc = void (*)(Camera2D&, Player&, std::vector<EnvItem>&, float, int, int);
	std::vector<CameraUpdaterFunc> cameraUpdaters = {
		UpdateCameraCenter,
		UpdateCameraCenterInsideMap,
		UpdateCameraCenterSmoothFollow,
		UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
	};

	int cameraOption = 0;
	std::vector<std::string> cameraDescriptions = {
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; update player center vertically after landing",
        "Player push camera on getting too close to screen edge"
    };

	SetTargetFPS(60);

	// Main game loop
	while (!WindowShouldClose()) {
		// update
		float deltaTime = GetFrameTime();
		player->Update(envItems, deltaTime);

		camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
		else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

		if (IsKeyPressed(KEY_R)) {
			camera.zoom = 1.0f;
			player->position = { 400, 200 };
		}

		if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1) % cameraUpdaters.size();

		// Call update camera function by its pointer
		cameraUpdaters[cameraOption](camera, *player, envItems, deltaTime, screenWidth, screenHeight);

		// Draw
		BeginDrawing();
			ClearBackground(LIGHTGRAY);
			BeginMode2D(camera);
				for (const auto& item: envItems) {
					DrawRectangleRec(item.rect, item.color);
				}

				player->Draw();

			EndMode2D();

			DrawText("Controls:", 20, 20, 10, BLACK);
			DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
			DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
			DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
			DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
			DrawText("Current camera mode:", 20, 120, 10, BLACK);
			DrawText(cameraDescriptions[cameraOption].c_str(), 40, 140, 10, DARKGRAY);

		EndDrawing();
	}

	// De-Initialization
	CloseWindow();
	return 0;
}

void UpdateCameraCenter(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height) {
	camera.offset = { width / 2.0f, height / 2.0f };
	camera.target = player.position;
}

void UpdateCameraCenterInsideMap(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height) {
	camera.target = player.position;
	camera.offset = { width / 2.0f, height / 2.0f };
	float minX = 1000;
	float minY = 1000;
	float maxX = -1000;
	float maxY = -1000;

	for (const auto& ei: envItems) {
		minX = fminf(ei.rect.x, minX);
		maxX = fmaxf(ei.rect.x + ei.rect.width, maxX);
		minY = fminf(ei.rect.y, minY);
		maxY = fmaxf(ei.rect.y + ei.rect.height, maxY);
	}

	Vector2 max = GetWorldToScreen2D({ maxX, maxY }, camera);
	Vector2 min = GetWorldToScreen2D({ minX, minY }, camera);

	if (max.x < width) camera.offset.x = width - (max.x - width / 2);
    if (max.y < height) camera.offset.y = height - (max.y - height / 2);
    if (min.x > 0) camera.offset.x = width / 2 - min.x;
    if (min.y > 0) camera.offset.y = height / 2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height) {
	static float minSpeed = 30;
	static float minEffectLength = 10;
	static float fractionSpeed = 0.8f;

	camera.offset = { width / 2.0f, height / 2.0f };
	Vector2 diff = Vector2Subtract(player.position, camera.target);
	float length = Vector2Length(diff);

	if (length > minEffectLength) {
		float speed = fmaxf(fractionSpeed * length, minSpeed);
		camera.target = Vector2Add(camera.target, Vector2Scale(diff, speed * delta / length));
	}
}

void UpdateCameraEvenOutOnLanding(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height) {
	static float evenOutSpeed = 700;
	static bool eveningOut = false;
	static float evenOutTarget;

	camera.offset = { width / 2.0f, height / 2.0f };
	camera.target.x = player.position.x;

	if (eveningOut) {
		if (evenOutTarget > camera.target.y) {
			camera.target.y += evenOutSpeed * delta;

			if (camera.target.y > evenOutTarget) {
				camera.target.y = evenOutTarget;
				eveningOut = false;
			}
		}
		else {
			camera.target.y -= evenOutSpeed * delta;

			if (camera.target.y < evenOutTarget) {
				camera.target.y = evenOutTarget;
				eveningOut = false;
			}
		}
	}
	else {
		if (player.canJump && (player.speed == 0) && (player.position.y != camera.target.y)) {
			eveningOut = true;
			evenOutTarget = player.position.y;
		}
	}
}

void UpdateCameraPlayerBoundsPush(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height) {
	static Vector2 bbox = { 0.2f, 0.2f };

	Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, camera);
    camera.offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

	if (player.position.x < bboxWorldMin.x) camera.target.x = player.position.x;
    if (player.position.y < bboxWorldMin.y) camera.target.y = player.position.y;
    if (player.position.x > bboxWorldMax.x) camera.target.x = bboxWorldMin.x + (player.position.x - bboxWorldMax.x);
    if (player.position.y > bboxWorldMax.y) camera.target.y = bboxWorldMin.y + (player.position.y - bboxWorldMax.y);
}
