#include "System_Physics.h"

namespace air {
	void C_RigidBody::setFixedRotation(bool _fix) {
		m_FixedRotation = _fix;

		if (h_body) {
			h_body->SetFixedRotation(_fix);
		}
	}

	void C_RigidBody::setMass(float _mass){
		m_mass = _mass;

		if (_gameObject.hasComponent<C_Collider_Box2d>()) {
			auto& c_box2d = _gameObject.getComponent<C_Collider_Box2d>();

			float area = c_box2d.size.x * c_box2d.size.y;

			h_body->GetFixtureList()->SetDensity(m_mass / area);
		}
	}
		
	void C_RigidBody::setCollisionCallback(void (*rg)(C_RigidBody&)) {
		this->collisionEnter = rg;
	}

	void C_RigidBody::setLinearVelocity(glm::vec2 vel) {
		if(h_body)
			h_body->SetLinearVelocity({ vel.x, vel.y });
	}

	glm::vec2 C_RigidBody::getLinearVelocity() {
		if (h_body) {
			auto _vl = h_body->GetLinearVelocity();
			return glm::vec2(_vl.x, _vl.y);
		}
		else return glm::vec2(0, 0);
	}

	void C_RigidBody::applyLenearImpulceToCenter(glm::vec2 vel) {
		if(h_body)
			h_body->ApplyLinearImpulseToCenter({ vel.x, vel.y }, true);
	}

	void C_RigidBody::setTransform(glm::vec2 _pos, float _angle) {
		if (h_body) 
			h_body->SetTransform({ _pos.x, _pos.y }, _angle);
		
	}

	const b2Transform& C_RigidBody::getTransform() {
		return h_body->GetTransform();
	}

	float C_RigidBody::getMass() {
		if (h_body)
			return h_body->GetMass();
	}

	void C_RigidBody::setGravityScale(float _gravityScale) {
		m_gravityScale = _gravityScale;
		if (h_body) 
			h_body->SetGravityScale(m_gravityScale);
	}
}