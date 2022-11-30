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
public:
	uint32_t seed;
	Tile** chunk_data = nullptr;
	bool active = false;

	Chunk();

	void setBlock(int _x, int _y, uint16_t id);
	Tile& getBlock(int _x, int _y);
	void setActive(bool);
	bool isActive();
	
	~Chunk();
};

