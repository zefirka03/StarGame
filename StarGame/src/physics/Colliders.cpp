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




	bool C_AABoundingBox::isCollide(Collider const& other) const {
		return other.isCollide(*this);
	}

	bool C_AABoundingBox::isCollide(C_AABoundingBox const& other) const {
		glm::vec2 borders0_x = glm::vec2(this->transform.position.x - this->transform.origin.x, this->transform.position.x - this->transform.origin.x + this->transform.size.x);
		glm::vec2 borders0_y = glm::vec2(this->transform.position.y - this->transform.origin.y, this->transform.position.y - this->transform.origin.y + this->transform.size.y);
		glm::vec2 borders1_x = glm::vec2(other.transform.position.x - other.transform.origin.x, other.transform.position.x - other.transform.origin.x + other.transform.size.x);
		glm::vec2 borders1_y = glm::vec2(other.transform.position.y - other.transform.origin.y, other.transform.position.y - other.transform.origin.y + other.transform.size.y);
		
		if (borders0_x.y > borders1_x.x && borders0_x.x < borders1_x.y &&
			borders0_y.y > borders1_y.x && borders0_y.x < borders1_y.y) return true;
		return false;
	}

	bool C_AABoundingBox::isCollide(C_BoundingBox const& other) const {
		return false;
	}


	box C_AABoundingBox::getBoundingBox() const {
		return box { 
			{this->transform.position.x - this->transform.origin.x, this->transform.position.x - this->transform.origin.x + this->transform.size.x}, 
			{this->transform.position.y - this->transform.origin.y, this->transform.position.y - this->transform.origin.y + this->transform.size.y}
		};
	}

	aabox C_AABoundingBox::getAABBox() const {
		return aabox{
			{this->transform.position.x - this->transform.origin.x, this->transform.position.y - this->transform.origin.y},
			transform.size
		};
	}



	bool C_BoundingBox::isCollide(Collider const& other) const {
		return other.isCollide(*this);
	}

	bool C_BoundingBox::isCollide(C_AABoundingBox const& other) const {
		return true;
	}

	bool C_BoundingBox::isCollide(C_BoundingBox const& other) const {
		glm::mat4 view1 = glm::rotate(glm::mat4(1), -transform.rotation, glm::vec3(0, 0, 1));

		glm::vec4 points_1[4] = {
			{glm::vec4(transform.position, 0) + view1 * glm::vec4(0 - transform.origin.x, 0 - transform.origin.y, 0, 1) },
			{glm::vec4(transform.position, 0) + view1 * glm::vec4(transform.size.x - transform.origin.x, 0 - transform.origin.y, 0, 1) },
			{glm::vec4(transform.position, 0) + view1 * glm::vec4(transform.size.x - transform.origin.x, transform.size.y - transform.origin.y, 0, 1) },
			{glm::vec4(transform.position, 0) + view1 * glm::vec4(0 - transform.origin.x, transform.size.y - transform.origin.y, 0, 1) }
		};

		glm::mat4 view2 = glm::rotate(glm::mat4(1), -other.transform.rotation, glm::vec3(0, 0, 1));

		glm::vec4 points_2[4] = {
			{glm::vec4(other.transform.position, 0) + view2 * glm::vec4(0 - other.transform.origin.x, 0 - other.transform.origin.y, 0, 1) },
			{glm::vec4(other.transform.position, 0) + view2 * glm::vec4(other.transform.size.x - other.transform.origin.x, 0 - other.transform.origin.y, 0, 1) },
			{glm::vec4(other.transform.position, 0) + view2 * glm::vec4(other.transform.size.x - other.transform.origin.x, other.transform.size.y - other.transform.origin.y, 0, 1) },
			{glm::vec4(other.transform.position, 0) + view2 * glm::vec4(0 - other.transform.origin.x, other.transform.size.y - other.transform.origin.y, 0, 1) }
		};

		for (int shape = 0; shape < 2; ++shape) {
			if (shape == 1)
				std::swap(points_1, points_2);
			for (int a = 0; a < 2; ++a) {
				int b = (a + 1) % 4;
				glm::vec2 _axis = { points_2[b].x - points_2[a].x, points_2[b].y - points_2[a].y };
				_axis = glm::normalize(_axis);

				float _min1 = 999999, _max1 = -999999;
				for (int i = 0; i < 4; ++i) {
					float pr = _axis.x * points_1[i].x + _axis.y * points_1[i].y;
					_min1 = std::min(_min1, pr);
					_max1 = std::max(_max1, pr);
				}

				float _min2 = 999999, _max2 = -999999;
				for (int i = 0; i < 4; ++i) {
					float pr = _axis.x * points_2[i].x + _axis.y * points_2[i].y;
					_min2 = std::min(_min2, pr);
					_max2 = std::max(_max2, pr);
				}

				if (!(_max2 >= _min1 && _max1 >= _min2))
					return false;
			}
		}
		return true;
	}


	box C_BoundingBox::getBoundingBox() const {
		glm::mat4 view = glm::rotate(glm::mat4(1), -transform.rotation, glm::vec3(0, 0, 1));

		glm::vec4 pos1 = view * glm::vec4(0 - transform.origin.x, 0 - transform.origin.y, 0, 1);
		glm::vec4 pos2 = view * glm::vec4(transform.size.x - transform.origin.x, 0 - transform.origin.y, 0, 1);
		glm::vec4 pos3 = view * glm::vec4(transform.size.x - transform.origin.x, transform.size.y - transform.origin.y, 0, 1);
		glm::vec4 pos4 = view * glm::vec4(0 - transform.origin.x, transform.size.y - transform.origin.y, 0, 1);

		return box{
			{this->transform.position.x + std::min(std::min(pos1.x, pos2.x), std::min(pos3.x, pos4.x)), this->transform.position.x + std::max(std::max(pos1.x, pos2.x), std::max(pos3.x, pos4.x))},
			{this->transform.position.y + std::min(std::min(pos1.y, pos2.y), std::min(pos3.y, pos4.y)), this->transform.position.y + std::max(std::max(pos1.y, pos2.y), std::max(pos3.y, pos4.y))}
		};
	}

	aabox C_BoundingBox::getAABBox() const {
		
		box b = getBoundingBox();


		return aabox{
			{b.horizontal.x, b.vertical.x},
			{b.horizontal.y - b.horizontal.x, b.vertical.y - b.vertical.x}
		};
	}


	PhysicalBody::PhysicalBody(Collider* _collider) {
		collider = _collider;
	}

	PhysicalBody::~PhysicalBody() {
		delete collider;
	}
}