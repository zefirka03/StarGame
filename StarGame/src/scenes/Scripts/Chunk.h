#pragma once
#include "air.h"

#define CHUNK_SIZE 32

using namespace air;

struct Tile {
	Tile() = default;
	Tile(uint16_t _id) : id(_id) {};
	uint16_t id = 0;
	Entity entity_handle;
};


class Chunk{
	uint32_t seed;
	Tile** chunk_data = nullptr;

	Chunk() = default;
	
	~Chunk();
};

