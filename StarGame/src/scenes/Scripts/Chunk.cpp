#include "Chunk.h"



Chunk::Chunk() {
	if (!chunk_data) {
		chunk_data = new Tile*[CHUNK_SIZE];
		for (int x = 0; x < CHUNK_SIZE; ++x)
			chunk_data[x] = new Tile[CHUNK_SIZE];
	}
}

void Chunk::setBlock(int _x, int _y, uint16_t id) {
	if (_x >= CHUNK_SIZE || _y >= CHUNK_SIZE || _x < 0 || _y < 0) return;

	chunk_data[_x][_y] = Tile(id);
}

Tile& Chunk::getBlock(int _x, int _y) {
	if (_x >= CHUNK_SIZE || _y >= CHUNK_SIZE || _x < 0 || _y < 0) return chunk_data[0][0];

	return chunk_data[_x][_y];
}

void Chunk::setActive(int _act_depth) {
	activation_depth = _act_depth;
}
bool Chunk::isActive() {
	return (bool)activation_depth;
}

int Chunk::activationDepth() {
	return activation_depth;
}

Chunk::~Chunk() {
	if (chunk_data) {
		for (int x = 0; x < CHUNK_SIZE; ++x) 
			delete[] chunk_data[x];
		delete[] chunk_data;
	}
}