#pragma once

#include "../ecs/EntityComponent.h"
#include "../ecs/System.h"
#include "../ecs/Script.h"
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
		friend class Air_B2dContactListener;
		friend class _System_Physics;
	public:
		enum class type {
			Static = 0,
			Dynamic,
			Kinematic
		} Type = type::Static;
		
		void setCollisionCallback(void (*rg)(C_RigidBody&));
		void setFixedRotation(bool _fix);
		void setMass(float _mass);
		float getMass();
		void setLinearVelocity(glm::vec2 vel);
		glm::vec2 getLinearVelocity();
		void applyLenearImpulceToCenter(glm::vec2 vel);
		void setGravityScale(float _gravityScale);
		void setTransform(glm::vec2 _pos, float _angle);

		bool isSensor = false;
		b2Body* h_body = nullptr;
	private:
		std::function<void(C_RigidBody&)> collisionEnter = nullptr;
		
		bool m_FixedRotation = false;
		float m_gravityScale = 1;
		float m_mass = 1;
	};

	struct C_Collider_Box2d : public Component {
		glm::vec2 offset = { 0, 0 };
		glm::vec2 size = { 1.f, 1.f };

		float density = 1.0f;
		float friction = 0.0f;
		float restitution = 0.5f;
		float restitutionThreshold = 0.0f;
	};
}