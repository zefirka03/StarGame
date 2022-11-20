#pragma once

#include "../glIncl.h"
#include "../components/Transform.h"
#include "../ecs/EntityComponent.h"

#include <limits>

namespace air {
	class PhysicsSystem;
	class C_PhysicalBody;
	class PhysicsQuadTree;
	class AABoundingBox;
	class BoundingBox;

	struct collisionInfo {
		glm::vec2 separation;
	};

	struct collisionPair {
		C_PhysicalBody* body;
		collisionInfo info;
	};

	enum class body_type {
		solid,
		rigit,
		trigger
	};

	struct box {
		glm::vec2 horizontal; // up-left corner
		glm::vec2 vertical; // bottom-right corner
	};

	struct aabox {
		glm::vec2 pos; 
		glm::vec2 size; 
	};

	class Collider{
		friend class PhysicsSystem;
		friend class PhysicsQuadTree;
		friend class C_PhysicalBody;
		friend class AABoundingBox;
		friend class BoundingBox;

	public:
		virtual bool isCollide(Collider const& other, collisionInfo& info) const = 0;
		virtual bool isCollide(AABoundingBox const& other, collisionInfo& info) const = 0;
		virtual bool isCollide(BoundingBox const& other, collisionInfo& info) const = 0;
		virtual box getBoundingBox() const = 0;
		virtual aabox getAABBox() const = 0;
		virtual ~Collider() {};

		Transform2d& getTransform();
	protected:
		C_PhysicalBody* phisical_body_handle = nullptr;
		Transform2d transform;
	};

	class AABoundingBox : public Collider {
	public:
		bool isCollide(Collider const& other, collisionInfo& info) const override;
		bool isCollide(AABoundingBox const& other, collisionInfo& info) const override;
		bool isCollide(BoundingBox const& other, collisionInfo& info) const override;
		box getBoundingBox() const override;

		aabox getAABBox() const override;

		~AABoundingBox() {};
	};

	class BoundingBox : public Collider {
	public:
		bool isCollide(Collider const& other, collisionInfo& info) const override;
		bool isCollide(AABoundingBox const& other, collisionInfo& info) const override;
		bool isCollide(BoundingBox const& other, collisionInfo& info) const override;
		box getBoundingBox() const override;

		aabox getAABBox() const override;

		~BoundingBox() {};
	};


	class C_PhysicalBody : public Component {
		friend class PhysicsSystem;
	public:
		C_PhysicalBody(Collider* _collider);
		Collider& getCollider();
		~C_PhysicalBody();
		void setCollisionsCollect(bool _coll);
		std::vector<collisionPair> const& getColliders() const;
		C_PhysicalBody(C_PhysicalBody&& rhs)           // move constructor
			: colliders(rhs.colliders)
		{
			collect_collisions = rhs.collect_collisions;
			colliders = rhs.colliders;
		}
		body_type type;
		struct physical_properties {
			glm::vec2 velocity = glm::vec2(0, 0);
			glm::vec2 acceleration = glm::vec2(0, 200);
			float mass = 1;
		} properties;
	private:

		std::vector<collisionPair> colliders;
		bool collect_collisions = false;
		Collider* collider = nullptr;
	};

}