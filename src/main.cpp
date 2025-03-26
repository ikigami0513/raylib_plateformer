#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>

#include "Player.hpp"

// Module functions declaration
void UpdateCameraCenter(Camera2D& camera, Player& player, std::vector<EnvItem>& envItems, float delta, int width, int height);

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "2d camera");

	Texture2D idlePlayerTexture = LoadTexture("resources/character_templates/idle/player_idle_48_48.png");

	Player* player = new Player({ 400, 280 }, 0, false);
	std::vector<EnvItem> envItems = {
		{{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
        {{ 0, 400, 1000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
	};

	Camera2D camera = { player->position, { screenWidth / 2.0f, screenHeight / 2.0f }, 0.0f, 1.0f };

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

		// Call update camera function by its pointer
		UpdateCameraCenter(camera, *player, envItems, deltaTime, screenWidth, screenHeight);

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
