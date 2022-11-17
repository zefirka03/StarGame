#include <iostream>
#include "air.h"

//#include "scenes/Scene1.h"
//#include "scenes/Scene2.h"
//#include "scenes/Scene3.h"
//#include "scenes/Scene_Coll_detect.h"
#include "scenes/Scene_RenderTex.h"

using namespace air;

int main() {
	air::Game game("game", 1920, 1080,true);
	game.run(new Scene_RenderTex());
	
	return 0;
}