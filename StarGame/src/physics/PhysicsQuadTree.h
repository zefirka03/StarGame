#pragma once
#include "../glIncl.h"
#include "../ecs/air_ecs.h"
#include "../components/Transform2d.h"
#include "../render/Renderer2d.h"
#include "../core/Input.h"

#include "../ecs/Component.h"

#include <vector>

namespace air {
	struct C_BoundingBox : public Component {
		Transform2d transform;

		std::vector<C_BoundingBox*> colliders;
		bool check_collisions = false;
	};

	class PhysicsQuadTree {
	public:
		PhysicsQuadTree(glm::vec2 _size, glm::vec2 _pos, int _depth = 0) {
			size = _size;
			position = _pos;
			depth = _depth;
		}

		PhysicsQuadTree* getContainingTree(C_BoundingBox* _bbox) {
			glm::vec2 borders_x = glm::vec2(_bbox->transform.position.x - _bbox->transform.origin.x, _bbox->transform.position.x - _bbox->transform.origin.x + _bbox->transform.size.x);
			glm::vec2 borders_y = glm::vec2(_bbox->transform.position.y - _bbox->transform.origin.y, _bbox->transform.position.y - _bbox->transform.origin.y + _bbox->transform.size.y);

			if (position.x + size.x / 2.f > borders_x.y && position.y + size.y / 2.f > borders_y.y) {
				return north_west;
			}
			else if (position.x + size.x / 2.f <= borders_x.x && position.y + size.y / 2.f > borders_y.y) {
				return north_east;
			}
			else if (position.x + size.x / 2.f > borders_x.y && position.y + size.y / 2.f <= borders_y.x) {
				return south_west;
			}
			else if (position.x + size.x / 2.f <= borders_x.x && position.y + size.y / 2.f <= borders_y.x) {
				return south_east;
			}
			return this;
		}

		void separate() {										  
			north_west = new PhysicsQuadTree(size / glm::vec2(2), position + glm::vec2(0, 0), depth+1 );
			north_east = new PhysicsQuadTree(size / glm::vec2(2), position + glm::vec2(size.x/2, 0), depth + 1);
			south_east = new PhysicsQuadTree(size / glm::vec2(2), position + glm::vec2(size.x/2, size.y/2), depth + 1);
			south_west = new PhysicsQuadTree(size / glm::vec2(2), position + glm::vec2(0, size.y/2), depth + 1);
			separated = true;
		}

		void insert(C_BoundingBox* _bbox) {
			auto it = getContainingTree(_bbox);
			if (it != this && capacity > depth + 1) {
				if (it == nullptr) separate();
				getContainingTree(_bbox)->insert(_bbox);
				return;
			}
			boxes.push_back(_bbox);
		}

		bool contains(C_BoundingBox* _bbox) {
			glm::vec2 borders0_x = glm::vec2(_bbox->transform.position.x - _bbox->transform.origin.x, _bbox->transform.position.x - _bbox->transform.origin.x + _bbox->transform.size.x);
			glm::vec2 borders0_y = glm::vec2(_bbox->transform.position.y - _bbox->transform.origin.y, _bbox->transform.position.y - _bbox->transform.origin.y + _bbox->transform.size.y);
			glm::vec2 borders1_x = glm::vec2(position.x, position.x + size.x);
			glm::vec2 borders1_y = glm::vec2(position.y, position.y + size.y);

			if (borders0_x.y > borders1_x.x && borders0_x.x < borders1_x.y &&
				borders0_y.y > borders1_y.x && borders0_y.x < borders1_y.y) return true;
			return false;
		}

		void retrieve(C_BoundingBox* _bbox, std::vector<C_BoundingBox*>& _out) {
			if (separated) {
				if(north_east->contains(_bbox))
					north_east->retrieve(_bbox, _out);
				if (north_west->contains(_bbox))
					north_west->retrieve(_bbox, _out);
				if (south_east->contains(_bbox))
					south_east->retrieve(_bbox, _out);
				if (south_west->contains(_bbox))
					south_west->retrieve(_bbox, _out);
			}
			for (int i = 0; i < boxes.size(); i++) {
				auto bx = boxes[i];
				if (_bbox == bx) continue;
				glm::vec2 borders0_x = glm::vec2(_bbox->transform.position.x - _bbox->transform.origin.x, _bbox->transform.position.x - _bbox->transform.origin.x + _bbox->transform.size.x);
				glm::vec2 borders0_y = glm::vec2(_bbox->transform.position.y - _bbox->transform.origin.y, _bbox->transform.position.y - _bbox->transform.origin.y + _bbox->transform.size.y);
				glm::vec2 borders1_x = glm::vec2(bx->transform.position.x - bx->transform.origin.x, bx->transform.position.x - bx->transform.origin.x + bx->transform.size.x);
				glm::vec2 borders1_y = glm::vec2(bx->transform.position.y - bx->transform.origin.y, bx->transform.position.y - bx->transform.origin.y + bx->transform.size.y);

				if (borders0_x.y > borders1_x.x && borders0_x.x < borders1_x.y &&
					borders0_y.y > borders1_y.x && borders0_y.x < borders1_y.y) _out.push_back(boxes[i]);
			}
		}

		std::vector<C_BoundingBox*> getAllBoxes() {
			if (separated) {
				std::vector<C_BoundingBox*> out;
				if (north_east) {
					std::vector<C_BoundingBox*> b = north_east->getAllBoxes();
					for (int i = 0; i < b.size(); ++i)
						out.push_back(b[i]);
				}
				if (north_west) {
					std::vector<C_BoundingBox*> b = north_west->getAllBoxes();
					for (int i = 0; i < b.size(); ++i)
						out.push_back(b[i]);
				}
				if (south_east) {
					std::vector<C_BoundingBox*> b = south_east->getAllBoxes();
					for (int i = 0; i < b.size(); ++i)
						out.push_back(b[i]);
				}
				if (south_west) {
					std::vector<C_BoundingBox*> b = south_west->getAllBoxes();
					for (int i = 0; i < b.size(); ++i)
						out.push_back(b[i]);
				}
				return out;
			}
			else {
				return boxes;
			}
		}

		std::vector<PhysicsQuadTree*> getAllTrees() {
			std::vector<PhysicsQuadTree*> out;
			
			if (north_east) {
				std::vector<PhysicsQuadTree*> b = north_east->getAllTrees();
				for (int i = 0; i < b.size(); ++i)
					out.push_back(b[i]);
			}
			if (north_west) {
				std::vector<PhysicsQuadTree*> b = north_west->getAllTrees();
				for (int i = 0; i < b.size(); ++i)
					out.push_back(b[i]);
			}
			if (south_east) {
				std::vector<PhysicsQuadTree*> b = south_east->getAllTrees();
				for (int i = 0; i < b.size(); ++i)
					out.push_back(b[i]);
			}
			if (south_west) {
				std::vector<PhysicsQuadTree*> b = south_west->getAllTrees();
				for (int i = 0; i < b.size(); ++i)
					out.push_back(b[i]);
			}
			
			out.push_back(this);
			return out;

		}

		glm::vec2 getSize() {
			return size;
		}

		glm::vec2 getPosition() {
			return position;
		}

		~PhysicsQuadTree() {
			delete north_east;
			delete north_west;
			delete south_east;
			delete south_west;
		}
	private:
		int depth = 0;
		int capacity = 6;

		bool separated = false;

		glm::vec2 size;
		glm::vec2 position;
		std::vector<C_BoundingBox*> boxes;

		PhysicsQuadTree* north_west = nullptr;
		PhysicsQuadTree* north_east = nullptr;
		PhysicsQuadTree* south_west = nullptr;
		PhysicsQuadTree* south_east = nullptr;
	};



	class PhysicsSystem : public System {
	public:
		void init() override {
			rectRenderer = new Renderer2dRectangles(100000);
		}
		void update(float _deltaTime) override {
			C_Camera2d* main_camera = nullptr;

			reg->view<C_Camera2d>().each([&](auto& cam) {
				main_camera = &cam;
			});

			delete quadTree;
			quadTree = new PhysicsQuadTree(glm::vec2(8000, 8000), glm::vec2(-4000, -4000));

			std::vector<C_BoundingBox*> CheckCollisionQueue;
			reg->view<C_BoundingBox, C_Transform2d>().each([&](C_BoundingBox& bbox, C_Transform2d& transform) {
				rectRenderer->draw({ bbox.transform, glm::vec4(0,1,0,1),1});
				quadTree->insert(&bbox);
				bbox.colliders.clear();

				if (bbox.check_collisions)
					CheckCollisionQueue.push_back(&bbox);

			});

			for (int i = 0; i < CheckCollisionQueue.size(); ++i) {
				quadTree->retrieve(CheckCollisionQueue[i], CheckCollisionQueue[i]->colliders);
			}


			std::vector<PhysicsQuadTree*> bb = quadTree->getAllTrees();

			for (int i = 0; i < bb.size(); i++) {
				Transform2d tr;
				tr.position = glm::vec3(bb[i]->getPosition(),0);
				tr.size = bb[i]->getSize();
				rectRenderer->draw({ tr, glm::vec4(1,0,0,1), 2});
			}

			rectRenderer->submit(*main_camera);
		}
		void terminate() override {
			delete rectRenderer;
			delete quadTree;
		}
		PhysicsQuadTree* quadTree;
	private:
		Renderer2dRectangles* rectRenderer;
	};
}