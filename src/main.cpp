#include "Game.hpp"

int main() {
	Game* game = new Game(800, 450, "Raylib Plateformer");
	game->GameLoop();
	return 0;
}
