#pragma once
#include "air.h"

using namespace air;

struct Tile {
	Tile() = default;
	Tile(uint16_t _id) : id(_id) {};
	uint16_t id = 0;
	Entity entity_handle;
};

class S_WorldGeneration : public Script {
public:
	S_WorldGeneration(int _w, int _h);

	Tile MakeBlock(int _x, int _y);

	void DestroyBlock(int _x, int _y);
	void OnCreate() override;


	void OnUpdate(float _deltaTime);
	void OnDestroy()override;
	Tile** getWorld();
	~S_WorldGeneration();
private:
	int w = 10, h = 10;
	Tile** world = nullptr;
};