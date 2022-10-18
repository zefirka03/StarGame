#include <iostream>
#include "air.h"


int main() {
	air::Game game("game", 1280, 720);
	game.run();
	
	return 0;
}