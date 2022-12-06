#pragma once
#include "air.h"
#include "Chunk.h"
#include <unordered_set>

#include "S_PlayerController.h"

using namespace air;


class S_World : public Script {
private:
	void _renderChunks();
	void _clearChunk(chunk_pos _pos);
public:
	bool debug = false;

	S_World(int _w, int _h);

	Tile MakeBlock(int _x, int _y);

	void DestroyTile(tile_pos _pos);
	void Generate();
	void setPlayer(Entity _player);

	void updateChunksAboutPlayer(Entity _player);
	//true if something happend, false if not,
	//act_depth - its depth of activation
	bool ActivateChunk(int _x, int _y, int _act_depth);
	//true if something happend, false if not
	bool DeactivateChunk(int _x, int _y);
	glm::vec2 getChunkSize();

	chunk_pos getChunkPos(glm::vec2 _pos);
	chunk_pos normalizedChunkPos(chunk_pos _pos);

	tile_pos getTilePos(glm::vec2 _pos);
	tile_pos normalizedTilePos(tile_pos _pos);

	void OnCreate() override;
	void OnUpdate(float _deltaTime) override;
	void OnDestroy() override;
	~S_World();

	Entity cam;
	char* map_image = nullptr;
private:
	Entity player;


	int w = 32, h = 32;
	int ch_w, ch_h;
	uint32_t seed;

	std::unordered_set<chunk_pos, cmp_chunk_pos> active_chunks;
	Chunk** chunks = nullptr;
};