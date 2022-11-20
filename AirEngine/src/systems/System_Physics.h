#pragma once

#include "../ecs/EntityComponent.h"
#include "../ecs/System.h"
#include "../render/Renderer2d.h"

#include <box2d/box2d.h>

namespace air {
	struct C_RigidBody;
	struct C_Collider_Box2d;

	struct Air_B2dContactListener;

	class _System_Physics : public System {
	public:
		void init() override;
		void update(float _deltaTime) override;
		void updateLast(float _deltaTime) override;
		void terminate() override;

		~_System_Physics() {
			std::cout << "Physics Terminated!\n";
			delete h_world;
			delete render;
		}
		bool debug = true;
	private:
		void instantiate_phisics_object(C_RigidBody& _rigid);

		Air_B2dContactListener* air_b2ContactListener_h;

		b2World* h_world = nullptr;
		RendererDebug* render;
	};

	struct C_RigidBody : public Component {
		enum class type {
			Static = 0,
			Dynamic,
			Kinematic
		} Type = type::Static;

		b2Body* h_body = nullptr;

		std::function<void(C_RigidBody&)> collisionEnter = nullptr;

		float mass = 1;
	};

	struct C_Collider_Box2d : public Component {
		glm::vec2 offset = { 0, 0 };
		glm::vec2 size = { 1.f, 1.f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.5f;
		float restitutionThreshold = 0.0f;
	};
}