#pragma once
#include <stdio.h>

#include "../ecs/EntityComponent.h"
#include "../ecs/System.h"
#include "../ecs/Script.h"
#include "../render/Renderer2d.h"

#include <box2d/box2d.h>

namespace air {
	struct C_RigidBody;
	struct C_Collider_Box2d;

	struct Air_B2dContactListener;
	

	class Physics_debugDraw : public b2Draw
	{
	public:

		Physics_debugDraw();
		~Physics_debugDraw();

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);
		void DrawPoint(const b2Vec2& p, float size, const b2Color& color);

		void Flush(C_Camera2d* cam);

	private:
		RendererDebug* render;
	};

	class _System_Physics : public System {
	public:
		void init() override;
		void update(float _deltaTime) override;
		void updateLast(float _deltaTime) override;
		void terminate() override;

		~_System_Physics() {
			printf( "Physics Terminated!\n");
			delete h_world;
		}
		bool debug = false;
	private:
		void instantiate_phisics_object(C_RigidBody& _rigid);

		Air_B2dContactListener* air_b2ContactListener_h;

		b2World* h_world = nullptr;
		Physics_debugDraw* debugDraw;
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
		const b2Transform& getTransform();

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