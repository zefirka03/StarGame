#pragma once
#include "../glIncl.h"
#include "../components/Transform.h"
#include "../ecs/EntityComponent.h"

namespace air {
	class PhysicsSystem;
	class PhysicalBody;
	class C_AABoundingBox;
	class C_BoundingBox;

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
		friend class C_AABoundingBox;
		friend class C_BoundingBox;

	public:
		virtual bool isCollide(Collider const& other) const = 0;
		virtual bool isCollide(C_AABoundingBox const& other) const = 0;
		virtual bool isCollide(C_BoundingBox const& other) const = 0;
		virtual box getBoundingBox() const = 0;
		virtual aabox getAABBox() const = 0;

		void setCollisionsCollect(bool _coll);
		std::vector<Collider*> const&	getColliders() const;
		Transform2d&				getTransform();
	protected:
		std::vector<Collider*> colliders;
		bool collect_collisions = false;
		Transform2d transform;
	};

	class C_AABoundingBox : public Collider {
	public:
		bool isCollide(Collider const& other) const override;
		bool isCollide(C_AABoundingBox const& other) const override;
		bool isCollide(C_BoundingBox const& other) const override;
		box getBoundingBox() const override;

		aabox getAABBox() const override;
	};

	class C_BoundingBox : public Collider {
	public:
		bool isCollide(Collider const& other) const override;
		bool isCollide(C_AABoundingBox const& other) const override;
		bool isCollide(C_BoundingBox const& other) const override;
		box getBoundingBox() const override;

		aabox getAABBox() const override;
	};


	class PhysicalBody : public Component {
	public:
		PhysicalBody(Collider* _collider);

		~PhysicalBody();
	private:
		Collider* collider;
	};

}