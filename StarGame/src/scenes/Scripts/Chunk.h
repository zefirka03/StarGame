#pragma once
#include "air.h"

#define CHUNK_SIZE 32
#define TILE_SIZE 16

using namespace air;

struct Tile {
	Tile() = default;
	Tile(uint16_t _id) : id(_id) {};
	Entity entity_handle;
	uint16_t id = 0;
};

typedef uint16_t tile_t;

struct tile_pos {
	int x;
	int y;

	tile_pos() = default;
	tile_pos(int _x, int _y) : x(_x), y(_y) {};
	bool operator==(const tile_pos& a) const {
		return a.x == x && a.y == y;
	}
};

struct chunk_pos {
	int x;
	int y;

	chunk_pos() = default;
	chunk_pos(int _x, int _y) : x(_x), y(_y) {};
	bool operator==(const chunk_pos& a) const {
		return a.x == x && a.y == y;
	}
};

struct cmp_chunk_pos {
	size_t operator()(const chunk_pos& a) const {
		return  13 * a.x + 73731 * a.y;
	}
};

class Chunk{
	friend class S_World;
private:
	uint32_t seed;
	Tile** chunk_data = nullptr;
	int activation_depth = 0;
	chunk_pos pos;

public:

	Chunk();

	void setBlock(int _x, int _y, uint16_t id);
	Tile& getBlock(int _x, int _y);
	void setActive(int _act_depth);
	bool isActive();
	int activationDepth();
	
	~Chunk();
};

