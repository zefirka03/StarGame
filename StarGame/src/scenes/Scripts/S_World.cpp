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
	val += PerlinNoise1D(x * 16.62f, seed) * 0.18f;
	val += PerlinNoise1D(x * 37.72f, seed) * 0.17f;
	// With this octaves numeric derivative is unstable.
	//val += PerlinNoise1D(x * 64.72f)  * 0.17f;
	//val += PerlinNoise1D(x * 128.72f) * 0.07f;
	//val += PerlinNoise1D(x * 256.72f) * 0.03f;
	val += 4.0f;
	val *= 0.20f;
	return val;
}

float nfunc(float _x) {
	return 0.5 * sin(_x) + 0.25 * cos(2 * _x - 25) + 0.33f * sin(_x - 1.51) + 0.2f * sin(3 * _x - 1.42);
}

void S_World::DestroyBlock(int _x, int _y) {
	if (_x >= w || _y >= h || _y < 0 || _x < 0) return;

	int chunk_pos_x = _x / CHUNK_SIZE;
	int chunk_pos_y = _y / CHUNK_SIZE;

	Chunk& cur_chunk = chunks[chunk_pos_x][chunk_pos_y];

	if (!cur_chunk.isActive()) return;

	int chunk_local_pos_x = _x % CHUNK_SIZE;
	int chunk_local_pos_y = _y % CHUNK_SIZE;

	Tile& cur_tile = cur_chunk.getBlock(chunk_local_pos_x, chunk_local_pos_y);

	if (cur_tile.id > 0) {
		getScene()->destroyEntity(cur_tile.entity_handle);
		cur_tile.id = 0;
	}
}

void S_World::DeactivateBlock(int _x, int _y) {
	if (_x >= w || _y >= h || _y < 0 || _x < 0) return;

	int chunk_pos_x = _x / CHUNK_SIZE;
	int chunk_pos_y = _y / CHUNK_SIZE;

	Chunk& cur_chunk = chunks[chunk_pos_x][chunk_pos_y];

	int chunk_local_pos_x = _x % CHUNK_SIZE;
	int chunk_local_pos_y = _y % CHUNK_SIZE;

	Tile& cur_tile = cur_chunk.getBlock(chunk_local_pos_x, chunk_local_pos_y);

	if (cur_tile.id > 0) {
		getScene()->destroyEntity(cur_tile.entity_handle);
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
		int he = (h / 2) + (h / 5) * fbm_noise(nfunc(i) * 3, seed);
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

	//for (int x = 0; x < ch_w; ++x) 
	//	for (int y = 0; y < ch_h; ++y) 
	//		ActivateChunk(x, y);
}

void S_World::setPlayer(Entity _player) {
	player = _player;
}

void S_World::OnUpdate(float _deltaTime) {
	updateChunksAboutPlayer(player);
}

void S_World::updateChunksAboutPlayer(Entity _player) {
	int ch_x = _player.getComponent<C_Transform2d>().transform.position.x / (CHUNK_SIZE * 16);
	int ch_y = _player.getComponent<C_Transform2d>().transform.position.y / (CHUNK_SIZE * 16);

	const int border_size_x = 3;
	const int border_size_y = 2;

	bool fl = false;

	for (int x = 0; !fl && x < getChunkSize().x; ++x) {
		for (int y = 0; !fl && y < getChunkSize().y; ++y) {
			if (x > ch_x - border_size_x && x < ch_x + border_size_x &&
				y > ch_y - border_size_y && y < ch_y + border_size_y)
				fl |= ActivateChunk(x, y);
			else fl |= DeactivateChunk(x, y);
		}
	}
}

glm::vec2 S_World::getChunkSize() {
	return { ch_w, ch_h };
}


bool S_World::ActivateChunk(int _x, int _y) {
	if (_x >= ch_w || _y >= ch_h || _x < 0 || _y < 0) return false;

	Chunk& cur_chunk = chunks[_x][_y];

	if (cur_chunk.isActive()) return false;

	int chunk_pos_x = _x * CHUNK_SIZE;
	int chunk_pos_y = _y * CHUNK_SIZE;

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {

			int tile_pos_x = chunk_pos_x + x;
			int tile_pos_y = chunk_pos_y + y;

			Tile& cur_tile = cur_chunk.getBlock(x, y);

			if (cur_tile.id > 0) {
				Entity ent = getScene()->createEntity();

				auto& sp = ent.addComponent<C_Sprite>();
				auto& rg = ent.addComponent<C_RigidBody>();
				auto& cl = ent.addComponent<C_Collider_Box2d>();
				auto& tr = ent.getComponent<C_Transform2d>();

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
				rg.Type = C_RigidBody::type::Static;
				cl.size = tr.transform.size;
				cl.restitutionThreshold = 0;
				cl.restitution = 0;

				cur_tile.entity_handle = ent;
			}
		}
	}
	cur_chunk.setActive(true);

	return true;
}

bool S_World::DeactivateChunk(int _x, int _y) {
	if (_x >= ch_w || _y >= ch_h || _x < 0 || _y < 0) return false;

	Chunk& cur_chunk = chunks[_x][_y];

	if (!cur_chunk.isActive()) return false;

	int chunk_pos_x = _x * CHUNK_SIZE;
	int chunk_pos_y = _y * CHUNK_SIZE;

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {

			int tile_pos_x = chunk_pos_x + x;
			int tile_pos_y = chunk_pos_y + y;

			DeactivateBlock(tile_pos_x, tile_pos_y);
		}
	}

	cur_chunk.setActive(false);
	return true;
}



void S_World::OnDestroy() {
	for (int x = 0; x < ch_w; ++x) 
		delete[] chunks[x];
	delete[] chunks;
	std::cout << "World Destructed\n";
}

S_World::~S_World() {
	std::cout << "World\n";
}