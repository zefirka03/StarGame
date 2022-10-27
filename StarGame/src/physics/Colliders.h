#pragma once
#include "../glIncl.h"
#include "../components/Transform.h"
#include "../ecs/EntityComponent.h"

namespace air {
	
	class PhysicsSystem;
	class C_BoundingBox;

	struct box {
		glm::vec2 horizontal; // up-left corner
		glm::vec2 vertical; // bottom-right corner
	};

	class Collider : public Component {
		friend class PhysicsSystem;
	public:
		virtual bool isCollide(Collider const& other) const = 0;
		virtual bool isCollide(C_BoundingBox const& other) const = 0;
		virtual box getBoundingBox() const = 0;

		void setCollisionsCollect(bool _coll);
		std::vector<Collider*> const&	getColliders() const;
		Transform2d&				getTransform();
	protected:
		std::vector<Collider*> colliders;
		bool collect_collisions = false;
		Transform2d transform;
	};

	class C_BoundingBox : public Collider {
	public:
		bool isCollide(Collider const& other) const override;
		bool isCollide(C_BoundingBox const& other) const override;
		box getBoundingBox() const override;
	};

}