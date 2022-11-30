#pragma once
#include "air.h"
#include "Chunk.h"

#include "S_PlayerController.h"

using namespace air;

class S_World : public Script {
public:
	S_World(int _w, int _h);

	Tile MakeBlock(int _x, int _y);

	void DestroyBlock(int _x, int _y);
	void DeactivateBlock(int _x, int _y);
	void Generate();
	void setPlayer(Entity _player);

	void updateChunksAboutPlayer(Entity _player);
	//true if something happend, false if not
	bool ActivateChunk(int _x, int _y);
	//true if something happend, false if not
	bool DeactivateChunk(int _x, int _y);
	glm::vec2 getChunkSize();

	void OnCreate() override;
	void OnUpdate(float _deltaTime) override;
	void OnDestroy()override;
	~S_World();
private:
	Entity player;

	int w = 32, h = 32;
	int ch_w, ch_h;
	uint32_t seed;
	Chunk** chunks = nullptr;
};