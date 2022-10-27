#pragma once
#include "../glIncl.h"
#include "../ecs/air_ecs.h"
#include "../components/Transform.h"
#include "../render/Renderer2d.h"
#include "../core/Input.h"

#include "../components/Camera2d.h"

#include "Colliders.h"

#include <vector>

namespace air {

	class PhysicsQuadTree {
	public:
		PhysicsQuadTree(glm::vec2 _size, glm::vec2 _pos, int _depth = 0) {
			size = _size;
			position = _pos;
			depth = _depth;
		}

		PhysicsQuadTree* getContainingTree(Collider* _bbox) {
			box object_bound = _bbox->getBoundingBox();

			if (position.x + size.x / 2.f > object_bound.horizontal.y && position.y + size.y / 2.f > object_bound.vertical.y) {
				return north_west;
			}
			else if (position.x + size.x / 2.f <= object_bound.horizontal.x && position.y + size.y / 2.f > object_bound.vertical.y) {
				return north_east;
			}
			else if (position.x + size.x / 2.f > object_bound.horizontal.y && position.y + size.y / 2.f <= object_bound.vertical.x) {
				return south_west;
			}
			else if (position.x + size.x / 2.f <= object_bound.horizontal.x && position.y + size.y / 2.f <= object_bound.vertical.x) {
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

		void insert(Collider* _bbox) {
			auto it = getContainingTree(_bbox);
			if (it != this && capacity > depth + 1) {
				if (it == nullptr) separate();
				getContainingTree(_bbox)->insert(_bbox);
				return;
			}
			boxes.push_back(_bbox);
		}

		bool contains(Collider* _bbox) {
			box object_bound = _bbox->getBoundingBox();
			box quad_bound = box{ 
				{position.x, position.x + size.x},
				{position.y, position.y + size.y}
			};

			if (object_bound.horizontal.y > quad_bound.horizontal.x && object_bound.horizontal.x < quad_bound.horizontal.y &&
				object_bound.vertical.y > quad_bound.vertical.x && object_bound.vertical.x < quad_bound.vertical.y) 
				return true;
			return false;
		}

		void retrieve(Collider* _bbox, std::vector<Collider*>& _out) {
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
				if(_bbox->isCollide(*bx)) _out.push_back(boxes[i]);
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
		int capacity = 7;

		bool separated = false;

		glm::vec2 size;
		glm::vec2 position;
		std::vector<Collider*> boxes;

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

			std::vector<Collider*> CheckCollisionQueue;
			reg->view<C_BoundingBox, C_Transform2d>().each([&](C_BoundingBox& bbox, C_Transform2d& transform) {
				if(debug) rectRenderer->draw({ bbox.getTransform(), glm::vec4(0,1,0,1),1});

				quadTree->insert(&bbox);
				bbox.colliders.clear();
				
				if (bbox.collect_collisions)
					CheckCollisionQueue.push_back(&bbox);

			});

			for (int i = 0; i < CheckCollisionQueue.size(); ++i) {
				quadTree->retrieve(CheckCollisionQueue[i], CheckCollisionQueue[i]->colliders);
			}


			if (debug) {
				std::vector<PhysicsQuadTree*> bb = quadTree->getAllTrees();

				for (int i = 0; i < bb.size(); i++) {
					Transform2d tr;
					tr.position = glm::vec3(bb[i]->getPosition(), 0);
					tr.size = bb[i]->getSize();
					rectRenderer->draw({ tr, glm::vec4(1,0,0,1), 2 });
				}

				rectRenderer->submit(main_camera->camera);
			}
		}
		void terminate() override {
			delete rectRenderer;
			delete quadTree;
		}
		PhysicsQuadTree* quadTree;
		bool debug = false;
	private:
		Renderer2dRectangles* rectRenderer;
	};
}