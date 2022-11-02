#include <iostream>
#include "air.h"

#include "scenes/Scene1.h"
#include "scenes/Scene2.h"
#include "scenes/Scene3.h"
#include "scenes/Scene_Coll_detect.h"

using namespace air;

int main() {
	air::Game game("game", 1280, 720);
	game.run(new Scene_Coll_detect());
	
	return 0;
}