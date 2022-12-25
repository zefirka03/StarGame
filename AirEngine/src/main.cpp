#include <iostream>
#include "air.h"

//#include "scenes/Scene1.h"
//#include "scenes/Scene2.h"
//#include "scenes/Scene3.h"
//#include "scenes/Scene_Coll_detect.h"
//#include "scenes/Scene_RenderTex.h"
#include "scenes/Scene_try1.h"


using namespace air;

int main() {
	air::Game game("game", 1280, 720,false);
	game.run(new Scene_try1());
	
	return 0;
}