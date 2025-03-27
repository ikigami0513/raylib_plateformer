#include "Game.hpp"

Game::Game(int screenWidth, int screenHeight, std::string title) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    InitWindow(this->screenWidth, this->screenHeight, title.c_str());

    this->player = new Player({ 400, 280 }, 0, false);
    this->envItems = {
        {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
        {{ 0, 400, 1000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
    };

    this->camera = { player->position, { screenWidth / 2.0f, screenHeight / 2.0f }, 0.0f, 1.0f };

    SetTargetFPS(60);
}

void Game::GameLoop() {
    while (!WindowShouldClose()) {
        this->Update();
        this->Render();
    }

    // De-Initialization
    CloseWindow();
}

void Game::Update() {
    float deltaTime = GetFrameTime();
    this->player->Update(this->envItems, deltaTime);

    this->camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

    if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

    if (IsKeyPressed(KEY_R)) {
        this->camera.zoom = 1.0f;
        this->player->position = { 400, 200 };
    }

    this->UpdateCamera();
}

void Game::UpdateCamera() {
    this->camera.offset = { this->screenWidth / 2.0f, this->screenHeight / 2.0f };
    this->camera.target = this->player->position;
}

void Game::Render() {
    BeginDrawing();
        ClearBackground(LIGHTGRAY);

        BeginMode2D(this->camera);
            for (const auto& item: envItems) {
                DrawRectangleRec(item.rect, item.color);
            }

            this->player->Draw();
        EndMode2D();

        DrawText("Controls:", 20, 20, 10, BLACK);
        DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
        DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
        DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
    EndDrawing();
}