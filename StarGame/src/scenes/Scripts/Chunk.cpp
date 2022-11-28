#include "Chunk.h"




Chunk::~Chunk() {
	if (chunk_data) {
		for (int x = 0; x < CHUNK_SIZE; ++x) 
			delete[] chunk_data[x];
		delete[] chunk_data;
	}
}