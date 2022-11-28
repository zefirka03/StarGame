#include "S_World.h"


S_World::S_World(int _w, int _h) {
	w = _w;
	h = _h;

	seed = 67890u + rand() % 994237;
}

Tile S_World::MakeBlock(int _x, int _y) {
	Tile out;
	out.id = 1 + rand() % 3;
	if (out.id) {
		Entity ent = getScene()->createEntity();

		auto& sp = ent.addComponent<C_Sprite>();
		auto& rg = ent.addComponent<C_RigidBody>();
		auto& cl = ent.addComponent<C_Collider_Box2d>();
		auto& tr = ent.getComponent<C_Transform2d>();

		ent.getComponent<C_Tag>().tag = "Ground";
		auto& TM = getScene()->getTextureManager();

		glm::vec4 t_color = glm::vec4(0.8, 0.2, 0.2, 1);
		switch (out.id)
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
		tr.transform = Transform2d(glm::vec3(_x * 16, _y * 16, 0), glm::vec2(16, 16), 0, glm::vec2(0, 0));
		rg.Type = C_RigidBody::type::Static;
		cl.size = tr.transform.size;
		cl.restitutionThreshold = 0;
		cl.restitution = 0;

		out.entity_handle = ent;
	}
	return out;
}

void S_World::DestroyBlock(int _x, int _y) {
	if (_x >= w || _y >= h || _y < 0 || _x < 0) return;

	if (world[_x][_y].id) {
		getScene()->destroyEntity(world[_x][_y].entity_handle);
		world[_x][_y] = Tile(0);
	}
}

void S_World::OnCreate()  {
	Generate();
}

float fHu(uint32_t value, uint32_t seed)
{
	// Try different values.
	const uint32_t seed0 = seed;
	const uint32_t seed1 = 67890u ;

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
	return 0.5 * sin(_x) + 0.25 * cos(2 * _x - 25) + 0.33f * sin(_x - 1.51) + 0.2f * sin(3 * _x  - 1.42);
}

void S_World::Generate() {
	world = new Tile * [w];
	for (int x = 0; x < w; ++x)
		world[x] = nullptr;

	constexpr float PI		= glm::pi<float>();
	constexpr float TWO_PI	= 2 * glm::pi<float>();

	for (float i = 0; i <= TWO_PI; i += TWO_PI / (w + 1)) {
		int he = (h / 2) + (h / 5) *  fbm_noise(nfunc(i) * 3, seed);
		int x = (i / TWO_PI) * w;
		
		if (!world[x]) {
			world[x] = new Tile[h];
			for (int y = 0; y < he; ++y) {
				world[x][y] = MakeBlock(x, y);
			}
		}
	}
}

void S_World::OnUpdate(float _deltaTime) {

}
void S_World::OnDestroy() {
	for (int x = 0; x < w; ++x)
		delete[] world[x];
	delete[] world;
	std::cout << "World Destructed\n";
}
Tile** S_World::getWorld() {
	return world;
}
S_World::~S_World() {
	std::cout << "World\n";
}