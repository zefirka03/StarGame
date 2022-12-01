#include <iostream>
#include "air.h"

#include "scenes/Scene_try1.h"

using namespace air;

int main() {
	Game game("MyGame", 1280, 720);
	game.run(new Scene_try1());

	return 0;
}