#include "Colliders.h"

namespace air {
	void Collider::setCollisionsCollect(bool _coll) {
		collect_collisions = _coll;
	}
	std::vector<Collider*> const& Collider::getColliders() const {
		return colliders;
	}
	Transform2d& Collider::getTransform() {
		return transform;
	}




	bool C_BoundingBox::isCollide(Collider const& other) const {
		return other.isCollide(*this);
	}

	bool C_BoundingBox::isCollide(C_BoundingBox const& other) const {
		glm::vec2 borders0_x = glm::vec2(this->transform.position.x - this->transform.origin.x, this->transform.position.x - this->transform.origin.x + this->transform.size.x);
		glm::vec2 borders0_y = glm::vec2(this->transform.position.y - this->transform.origin.y, this->transform.position.y - this->transform.origin.y + this->transform.size.y);
		glm::vec2 borders1_x = glm::vec2(other.transform.position.x - other.transform.origin.x, other.transform.position.x - other.transform.origin.x + other.transform.size.x);
		glm::vec2 borders1_y = glm::vec2(other.transform.position.y - other.transform.origin.y, other.transform.position.y - other.transform.origin.y + other.transform.size.y);
		
		if (borders0_x.y > borders1_x.x && borders0_x.x < borders1_x.y &&
			borders0_y.y > borders1_y.x && borders0_y.x < borders1_y.y) return true;
		return false;
	}

	box C_BoundingBox::getBoundingBox() const {
		return box { 
			{this->transform.position.x - this->transform.origin.x, this->transform.position.x - this->transform.origin.x + this->transform.size.x}, 
			{this->transform.position.y - this->transform.origin.y, this->transform.position.y - this->transform.origin.y + this->transform.size.y}
		};
	}
}