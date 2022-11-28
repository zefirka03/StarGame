#pragma once
#include "air.h"
#include "Chunk.h"

using namespace air;

class S_World : public Script {
public:
	S_World(int _w, int _h);

	Tile MakeBlock(int _x, int _y);

	void DestroyBlock(int _x, int _y);
	void Generate();

	void OnCreate() override;
	void OnUpdate(float _deltaTime) override;
	void OnDestroy()override;
	Tile** getWorld();
	~S_World();
private:
	int w = 10, h = 10;
	uint32_t seed;
	Tile** world = nullptr;
	Chunk** chunks = nullptr;
};