#include "S_World.h"


S_WorldGeneration::S_WorldGeneration(int _w, int _h) {
	w = _w;
	h = _h;
}

Tile S_WorldGeneration::MakeBlock(int _x, int _y) {
	Tile out;
	out.id = rand() % 3;
	if (out.id) {
		Entity ent = getScene()->createEntity();

		auto& sp = ent.addComponent<C_Sprite>();
		auto& rg = ent.addComponent<C_RigidBody>();
		auto& cl = ent.addComponent<C_Collider_Box2d>();
		auto& tr = ent.getComponent<C_Transform2d>();

		ent.getComponent<C_Tag>().tag = "Ground";
		auto& TM = getScene()->getTextureManager();

		sp = C_Sprite(glm::vec4(0.8, 0.2, 0.2, 1), glm::vec4(0, 0, 1, 1), TM.getTexture("empty"));
		tr.transform = Transform2d(glm::vec3(_x * 16, _y * 16, 0), glm::vec2(16, 16), 0, glm::vec2(0, 0));
		rg.Type = C_RigidBody::type::Static;
		cl.size = tr.transform.size;
		cl.restitutionThreshold = 0;
		cl.restitution = 0;

		out.entity_handle = ent;
	}
	return out;
}

void S_WorldGeneration::DestroyBlock(int _x, int _y) {
	if (_x >= w || _y >= h || _y < 0 || _x < 0) return;
	//std::cout << "Destroyed: " << _x << " " << _y << "\n";
	//std::cout << "Handle: " << (int)world[_x][_y].entity_handle.entity_handle << '\n';
	if (world[_x][_y].id) {
		getScene()->destroyEntity(world[_x][_y].entity_handle);
		world[_x][_y] = Tile(0);
	}
}

void S_WorldGeneration::OnCreate()  {
	world = new Tile * [w];
	for (int x = 0; x < w; ++x) {
		world[x] = new Tile[h];
		for (int y = 0; y < h; ++y) {
			world[x][y] = MakeBlock(x, y);
		}
	}
}


void S_WorldGeneration::OnUpdate(float _deltaTime) {

}
void S_WorldGeneration::OnDestroy() {
	for (int x = 0; x < w; ++x)
		delete[] world[x];
	delete[] world;
	std::cout << "World Destructed\n";
}
Tile** S_WorldGeneration::getWorld() {
	return world;
}
S_WorldGeneration::~S_WorldGeneration() {
	std::cout << "World\n";
}