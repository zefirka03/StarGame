#include "S_World.h"


S_World::S_World(int _w, int _h) {
	w = _w;
	h = _h;

	seed = 67890u + rand() % 994237;
}

float fHu(uint32_t value, uint32_t seed)
{
	// Try different values.
	const uint32_t seed0 = seed;
	const uint32_t seed1 = 67890u;

	// Calculate hash.
	value += seed1;
	value *= 445593459u;
	value ^= seed0;

	// 2.0f / 4294967295.0f = 4.6566128730773926e-10   
	return float(value * value * value) * 4.6566128730773926e-10 - 1.0f;
}

float PerlinNoise1D(float u, uint32_t seed)
{
	// Fractional part.
	float fractU = u - floor(u);

	// Integer part.
	u = floor(u);

	// Smoothstep.
	float tU = fractU * fractU * (3.0f - 2.0f * fractU);

	// Random gradient vectors (1D).
	float gradient0 = fHu(uint32_t(u), seed);
	float gradient1 = fHu(uint32_t(u + 1.0f), seed);

	// Distance vectors (1D).
	float distance0 = fractU;
	float distance1 = fractU - 1.0f;

	// Dot products (1D).
	float dotProduct0 = gradient0 * distance0;
	float dotProduct1 = gradient1 * distance1;

	// Linear filter (lerp).
	return
		dotProduct0 * (1.0f - tU) +
		dotProduct1 * tU;
}

float fbm_noise(float x, uint32_t seed)
{
	float val = PerlinNoise1D(x, seed) * 6.6f;
	val += PerlinNoise1D(x * 2.3f, seed) * 2.62f;
	val += PerlinNoise1D(x * 4.32f, seed) * 0.47f;
	val += PerlinNoise1D(x * 8.672f, seed) * 0.17f;
	//val += PerlinNoise1D(x * 16.62f, seed) * 0.18f;
	//val += PerlinNoise1D(x * 37.72f, seed) * 0.17f;

	val += 4.0f;
	val *= 0.20f;
	return val;
}

float nfunc(float _x) {
	//return 0.5 * sin(_x) + 0.25 * cos(2 * _x - 25) + 0.33f * sin(_x - 1.51) + 0.2f * sin(3 * _x - 1.42);
	return 0.5 * sin(_x) + 0.25 * cos(2 * _x - 25) + 0.33f * sin(_x - 1.51) + 0.2f * sin(3 * _x - 1.42);
}

void S_World::DestroyTile(tile_pos _pos) {
	Chunk& cur_chunk = chunks[_pos.x / CHUNK_SIZE][_pos.y / CHUNK_SIZE];
	
	if (!cur_chunk.isActive()) return;

	Tile& cur_tile = cur_chunk.getBlock(_pos.x % CHUNK_SIZE, _pos.y % CHUNK_SIZE);

	if (cur_tile.id > 0) {
		getScene()->destroyEntity(cur_tile.entity_handle);
		cur_tile.id = 0;
	}
}



void S_World::OnCreate()  {
	Generate();
}




void S_World::Generate() {
	ch_w = (w + CHUNK_SIZE - 1) / CHUNK_SIZE;
	ch_h = (h + CHUNK_SIZE - 1) / CHUNK_SIZE;


	chunks = new Chunk*[ch_w];
	for (int x = 0; x < ch_w; ++x)
		chunks[x] = new Chunk[ch_h];

	constexpr float PI		= glm::pi<float>();
	constexpr float TWO_PI	= 2 * glm::pi<float>();

	std::vector<int> height_map(w, -1);

	for (float i = 0; i <= TWO_PI; i += TWO_PI / (w + 1)) {
		int he = (h / 2) + (h / 5) * fbm_noise(nfunc(i), seed);
		int x = (i / TWO_PI) * w;

		if (height_map[x] == -1) 
			height_map[x] = he;
	}

	for(int x = 0; x < w; ++x){
		for (int y = 0; y < height_map[x]; ++y) {
			int ch_x = x / CHUNK_SIZE;
			int ch_y = y / CHUNK_SIZE;	
			
			int ch_x_p = x % CHUNK_SIZE;
			int ch_y_p = y % CHUNK_SIZE;

			chunks[ch_x][ch_y].setBlock(ch_x_p, ch_y_p, 1);
		}
	}

	//
	if (!map_image) {
		map_image = new char[CHUNK_SIZE * CHUNK_SIZE * ch_w * ch_h];
		for (int x = 0; x < CHUNK_SIZE * CHUNK_SIZE * ch_w * ch_h; ++x)
			map_image[x] = 0;
		for (int x = 0; x < CHUNK_SIZE * ch_w; ++x) {
			for (int y = 0; y < CHUNK_SIZE * ch_h; ++y) {
				if (x < w) {
					if (y < height_map[x])
						map_image[ (CHUNK_SIZE * ch_h - y) * CHUNK_SIZE * ch_w + x ] = 255;
				}
			}
		}
		getScene()->getTextureManager().loadTextureByData(map_image, { CHUNK_SIZE * ch_w,  CHUNK_SIZE * ch_h, AIR_TEXTURE_R8, AIR_TEXTURE_RED }, "map");
	}
}

void S_World::setPlayer(Entity _player) {
	player = _player;

}

void S_World::OnUpdate(float _deltaTime) {
	if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
		player.getComponent<C_Transform2d>().transform.position = glm::vec3(Input::getCursorPos(cam.getComponent<C_Camera2d>().camera), 1);
		player.getComponent<C_RigidBody>().setTransform(Input::getCursorPos(cam.getComponent<C_Camera2d>().camera), 0);
	}

	updateChunksAboutPlayer(player);

	if (debug) _renderChunks();
}

void S_World::updateChunksAboutPlayer(Entity _player) {
	int ch_x = (int)floor(_player.getComponent<C_Transform2d>().transform.position.x / (CHUNK_SIZE * 16));
	int ch_y = (int)floor(_player.getComponent<C_Transform2d>().transform.position.y / (CHUNK_SIZE * 16));

	const int depth_1_border_size_x = 2;
	const int depth_1_border_size_y = 2;
	
	const int depth_2_border_size_x = 1;
	const int depth_2_border_size_y = 1;

	bool fl = false;
	
	for (int x = ch_x - depth_1_border_size_x; !fl && x <= ch_x + depth_1_border_size_x; ++x) {
		for (int y = ch_y - depth_1_border_size_y; !fl && y <= ch_y + depth_1_border_size_y; ++y) {
			if (x >= ch_x - depth_2_border_size_x && x <= ch_x + depth_2_border_size_x &&
				y >= ch_y - depth_2_border_size_x && y <= ch_y + depth_2_border_size_x)
				fl |= ActivateChunk(x, y, 2);
			else if (x >= ch_x - depth_1_border_size_x && x <= ch_x + depth_1_border_size_x &&
					 y >= ch_y - depth_1_border_size_y && y <= ch_y + depth_1_border_size_y)
				fl |= ActivateChunk(x, y, 1);
		}
	}

	for (auto it = active_chunks.begin(); it != active_chunks.end(); ++it) {
		int x = it->x;
		int y = it->y;

		if(	x < ch_x - depth_1_border_size_x || x > ch_x + depth_1_border_size_x ||
			y < ch_y - depth_1_border_size_y || y > ch_y + depth_1_border_size_y)
			DeactivateChunk(x, y);
	}
}

void S_World::_renderChunks() {
	
	for(auto it = active_chunks.begin(); it!=active_chunks.end(); ++it){
		int x = it->x;
		int y = it->y;
		
		chunk_pos nChunkPos = normalizedChunkPos({ x, y });

		switch (chunks[nChunkPos.x][nChunkPos.y].activationDepth())
		{
		case 2:
			getScene()->Debug().drawQuad({ x * CHUNK_SIZE * 16,y * CHUNK_SIZE * 16 }, glm::vec2(CHUNK_SIZE * 16), glm::vec4(0, 1, 0, 1));
			break;
		case 1:
			getScene()->Debug().drawQuad({ x * CHUNK_SIZE * 16,y * CHUNK_SIZE * 16 }, glm::vec2(CHUNK_SIZE * 16), glm::vec4(1, 1, 0, 1));
			break;
		case 0:
			getScene()->Debug().drawQuad({ x * CHUNK_SIZE * 16,y * CHUNK_SIZE * 16 }, glm::vec2(CHUNK_SIZE * 16), glm::vec4(1, 0, 0, 1));
			break;
		default:
			break;
		}
	}
	
}

glm::vec2 S_World::getChunkSize() {
	return { ch_w, ch_h };
}

chunk_pos S_World::getChunkPos(glm::vec2 _pos) {
	int ch_x = (int)floor(_pos.x / (CHUNK_SIZE * TILE_SIZE));
	int ch_y = (int)floor(_pos.y / (CHUNK_SIZE * TILE_SIZE));

	chunk_pos norm = normalizedChunkPos({ ch_x, ch_y });

	return norm;
}



chunk_pos S_World::normalizedChunkPos(chunk_pos _pos) {
	int local_chunk_x;
	if (_pos.x < 0) local_chunk_x = (ch_w - abs(_pos.x) % ch_w) % ch_w;
	else local_chunk_x = _pos.x % ch_w;
	int local_chunk_y;
	if (_pos.y < 0) local_chunk_y = (ch_h - abs(_pos.y) % ch_h) % ch_h;
	else local_chunk_y = _pos.y % ch_h;

	return { local_chunk_x, local_chunk_y };
}


tile_pos S_World::getTilePos(glm::vec2 _pos) {
	int ch_x = (int)floor(_pos.x / TILE_SIZE);
	int ch_y = (int)floor(_pos.y / TILE_SIZE);

	tile_pos norm = normalizedTilePos({ ch_x, ch_y });

	return norm;
}

tile_pos S_World::normalizedTilePos(tile_pos _pos) {
	int tile_size_x = ch_w * CHUNK_SIZE;
	int tile_size_y = ch_h * CHUNK_SIZE;

	int local_chunk_x;
	if (_pos.x < 0) local_chunk_x = (tile_size_x - abs(_pos.x) % tile_size_x) % tile_size_x;
	else local_chunk_x = _pos.x % tile_size_x;
	int local_chunk_y;
	if (_pos.y < 0) local_chunk_y = (tile_size_y - abs(_pos.y) % tile_size_y) % tile_size_y;
	else local_chunk_y = _pos.y % tile_size_y;

	return { local_chunk_x, local_chunk_y };
}


bool S_World::ActivateChunk(int _x, int _y, int _act_depth) {
	auto it = active_chunks.find({ _x, _y });
	if (it == active_chunks.end()) {
		it = active_chunks.insert({ _x, _y }).first;
	}

	chunk_pos nChunkPos = normalizedChunkPos({ _x, _y });

	Chunk& cur_chunk = chunks[nChunkPos.x][nChunkPos.y];

	if (cur_chunk.activationDepth() == _act_depth) return false;
	if (cur_chunk.activationDepth() != 0 ) {
		_clearChunk(nChunkPos);
	}

	int chunk_pos_x = _x * CHUNK_SIZE;
	int chunk_pos_y = _y * CHUNK_SIZE;
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			int tile_pos_x = chunk_pos_x + x;
			int tile_pos_y = chunk_pos_y + y;
			
			Tile& cur_tile = cur_chunk.getBlock(x, y);

			if (cur_tile.id > 0) {
				Entity ent = getScene()->createEntity();
				auto& tr = ent.getComponent<C_Transform2d>();

				if(_act_depth > 0){
					auto& sp = ent.addComponent<C_Sprite>();

					ent.getComponent<C_Tag>().tag = "Ground";
					auto& TM = getScene()->getTextureManager();

					glm::vec4 t_color = glm::vec4(0.8, 0.2, 0.2, 1);
					switch (cur_tile.id)
					{
					case 1:
						t_color = glm::vec4(0.2, 0.8, 0.2, 1);
						break;
					case 2:
						t_color = glm::vec4(0.8, 0.2, 0.2, 1);
						break;
					case 3:
						t_color = glm::vec4(0.2, 0.2, 0.8, 1);
						break;
					default:
						break;
					}

					sp = C_Sprite(t_color, glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
					tr.transform = Transform2d(glm::vec3(tile_pos_x * 16, tile_pos_y * 16, 0), glm::vec2(16, 16), 0, glm::vec2(0, 0));
				}
				if (_act_depth > 1) {
					auto& rg = ent.addComponent<C_RigidBody>();
					auto& cl = ent.addComponent<C_Collider_Box2d>();
					rg.Type = C_RigidBody::type::Static;
					cl.size = tr.transform.size;
					cl.restitutionThreshold = 0;
					cl.restitution = 0;
				}
				cur_tile.entity_handle = ent;
			}
		}
	}
	cur_chunk.setActive(_act_depth);
	cur_chunk.pos = { _x, _y };

	return true;
}

bool S_World::DeactivateChunk(int _x, int _y) {
	auto it = active_chunks.find({ _x, _y });
	if (it == active_chunks.end())
		return false;

	chunk_pos nChunkPos = normalizedChunkPos({ _x, _y });

	Chunk& cur_chunk = chunks[nChunkPos.x][nChunkPos.y];

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			Tile& cur_tile = cur_chunk.getBlock(x, y);
			if (cur_tile.id > 0) {
				getScene()->destroyEntity(cur_tile.entity_handle);
			}
		}
	}
	active_chunks.erase(it);
	
	cur_chunk.setActive(0);

	return true;
}

void S_World::_clearChunk(chunk_pos _pos) {
	_pos = normalizedChunkPos(_pos);

	Chunk& cur_chunk = chunks[_pos.x][_pos.y];

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			Tile& cur_tile = cur_chunk.getBlock(x, y);
			if (cur_tile.id > 0) {
				getScene()->destroyEntity(cur_tile.entity_handle);
			}
		}
	}
	cur_chunk.setActive(0);
}


void S_World::OnDestroy() {
	for (int x = 0; x < ch_w; ++x) 
		delete[] chunks[x];
	delete[] chunks;
	printf("World Destructed\n");
}

S_World::~S_World() {
	delete map_image;
	printf("World\n");
}