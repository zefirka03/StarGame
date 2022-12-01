#include "System_Physics.h"

namespace air {
	static b2BodyType FromBox2dToAirBodyType(C_RigidBody::type _type) {
		switch (_type)
		{
		case C_RigidBody::type::Dynamic: return b2BodyType::b2_dynamicBody;
		case C_RigidBody::type::Kinematic: return b2BodyType::b2_kinematicBody;
		case C_RigidBody::type::Static: return b2BodyType::b2_staticBody;
		}
		return b2BodyType::b2_staticBody;
	}

	class Air_B2dContactListener : public b2ContactListener {
	public:
		Scene* scene;
		void BeginContact(b2Contact* contact) override {
			auto rb1 = (&Entity((entt::entity)contact->GetFixtureA()->GetBody()->GetUserData().pointer, scene).getComponent<C_RigidBody>());
			auto rb2 = (&Entity((entt::entity)contact->GetFixtureB()->GetBody()->GetUserData().pointer, scene).getComponent<C_RigidBody>());

			if (rb1->_gameObject.hasComponent<_C_NativeScriptComponent>()) {
				_C_NativeScriptComponent& nsc = rb1->_gameObject.getComponent<_C_NativeScriptComponent>();
				for (auto it = nsc.Instances.begin(); it != nsc.Instances.end(); ++it) {
					(*it)->OnCollisionEnter(*rb2);
				}
			}

			if (rb2->_gameObject.hasComponent<_C_NativeScriptComponent>()) {
				_C_NativeScriptComponent& nsc = rb2->_gameObject.getComponent<_C_NativeScriptComponent>();
				for (auto it = nsc.Instances.begin(); it != nsc.Instances.end(); ++it) {
					(*it)->OnCollisionEnter(*rb1);
				}
			}
		}
		void EndContact(b2Contact* contact) override {
			auto rb1 = (&Entity((entt::entity)contact->GetFixtureA()->GetBody()->GetUserData().pointer, scene).getComponent<C_RigidBody>());
			auto rb2 = (&Entity((entt::entity)contact->GetFixtureB()->GetBody()->GetUserData().pointer, scene).getComponent<C_RigidBody>());

			if (rb1->_gameObject.hasComponent<_C_NativeScriptComponent>()) {
				_C_NativeScriptComponent& nsc = rb1->_gameObject.getComponent<_C_NativeScriptComponent>();
				for (auto it = nsc.Instances.begin(); it != nsc.Instances.end(); ++it) {
					(*it)->OnCollisionEnd(*rb2);
				}
			}

			if (rb2->_gameObject.hasComponent<_C_NativeScriptComponent>()) {
				_C_NativeScriptComponent& nsc = rb2->_gameObject.getComponent<_C_NativeScriptComponent>();
				for (auto it = nsc.Instances.begin(); it != nsc.Instances.end(); ++it) {
					(*it)->OnCollisionEnd(*rb1);
				}
			}
		}
		
	};

	void _System_Physics::instantiate_phisics_object(C_RigidBody& _rigid) {
		Entity& gameObject = _rigid._gameObject;

		auto& tr = gameObject.getComponent<C_Transform2d>();

		b2BodyDef bodyDef;
		bodyDef.type = FromBox2dToAirBodyType(_rigid.Type);
		bodyDef.position.Set(tr.transform.position.x, tr.transform.position.y);
		bodyDef.angle = -tr.transform.rotation;

		b2Body* body = h_world->CreateBody(&bodyDef);
		body->GetUserData().pointer = (uintptr_t)_rigid._gameObject.entity_handle;
		body->SetFixedRotation(_rigid.m_FixedRotation);
		body->SetGravityScale(_rigid.m_gravityScale);
		
		_rigid.h_body = body;
		
		if (gameObject.hasComponent<C_Collider_Box2d>()) {
			auto& c_box2d = gameObject.getComponent<C_Collider_Box2d>();
			b2PolygonShape shape;

			shape.SetAsBox(c_box2d.size.x / 2.f, c_box2d.size.y / 2.f, { c_box2d.size.x/2 - tr.transform.origin.x , c_box2d.size.y/2 - tr.transform.origin.y  }, 0);
			
			b2Vec2 vs[4];
			vs[3].Set(-tr.transform.origin.x, -tr.transform.origin.y);
			vs[2].Set(-tr.transform.origin.x, -tr.transform.origin.y + c_box2d.size.y);
			vs[1].Set(-tr.transform.origin.x + c_box2d.size.x, -tr.transform.origin.y + c_box2d.size.y);
			vs[0].Set(-tr.transform.origin.x + c_box2d.size.x, -tr.transform.origin.y);


			b2ChainShape chain;
			chain.CreateLoop(vs, 4);

			float area = c_box2d.size.x * c_box2d.size.y;

			b2FixtureDef fixture;
			
			if(bodyDef.type == b2BodyType::b2_staticBody)
				fixture.shape = &chain;
			else  fixture.shape = &shape;

			fixture.isSensor = _rigid.isSensor;
			fixture.density = _rigid.m_mass / area;
			fixture.friction = c_box2d.friction;
			fixture.restitution = c_box2d.restitution;
			fixture.restitutionThreshold = c_box2d.restitutionThreshold;

			body->CreateFixture(&fixture);
			body->ResetMassData();
		}
	}

	void _System_Physics::init()  {
		air_b2ContactListener_h = new Air_B2dContactListener();

		//render = new RendererDebug(50000);

		h_world = new b2World(b2Vec2(0.f, -9.8f*25));
		h_world->SetContactListener(air_b2ContactListener_h);
		air_b2ContactListener_h->scene = scene;

		debugDraw = new Physics_debugDraw();
		h_world->SetDebugDraw(debugDraw);
		debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);

		auto view = reg->view<C_RigidBody>();
		view.each([&](C_RigidBody& rigid) {
			if (!rigid.h_body)
				this->instantiate_phisics_object(rigid);
		});
	}


	void _System_Physics::update(float _deltaTime)  {

		h_world->Step(_deltaTime, 4, 4);

		reg->view<C_RigidBody>().each([&](C_RigidBody& rigid) {
			if (rigid._gameObject.hasComponent<_C_Destroyed>() && rigid.h_body != nullptr) {
				h_world->DestroyBody(rigid.h_body);
				return;
			}
			if (!rigid.h_body)
				this->instantiate_phisics_object(rigid);

				Entity& gameObject = rigid._gameObject;

				auto& tr = gameObject.getComponent<C_Transform2d>();

				const auto& n_pos = rigid.h_body->GetPosition();

				tr.transform.position = { n_pos.x, n_pos.y, 0 };
				tr.transform.rotation = -rigid.h_body->GetAngle();

				//if (rigid.isSensor) {
				//	auto contactss = rigid.h_body->GetContactList();
				//	if (contactss) {
				//		auto contacts = contactss->contact;
				//		while (contacts) {
				//			auto rg = (C_RigidBody*)contacts->GetFixtureA()->GetBody()->GetUserData().pointer;
				//			std::cout << rg->_gameObject.getComponent<C_Tag>().tag << '\n';
				//			contacts = contacts->GetNext();
				//		}
				//	}
				//}
		});
	}

	void _System_Physics::updateLast(float _deltaTime) {
		if (debug) {
			
			C_Camera2d* main_camera = nullptr;

			reg->view<C_Camera2d>().each([&](auto& cam) {
				if(cam.camera.main)
					main_camera = &cam;
			});
			
			h_world->DebugDraw();
			debugDraw->Flush(main_camera);
			
			//reg->view<C_Collider_Box2d>().each([&](C_Collider_Box2d& coll) {
			//	auto rb = coll._gameObject.getComponent<C_RigidBody>().h_body;
			//
			//	glm::vec4 _color(1);
			//	if (!coll._gameObject.getComponent<C_RigidBody>().isSensor)
			//		_color = glm::vec4(0, 1, 0, 1);
			//	else _color = glm::vec4(0, 0, 1, 1);
			//
			//	debugDraw->render->drawQuad(Transform2d(glm::vec3(rb->GetPosition().x, rb->GetPosition().y, 0), coll.size, -rb->GetAngle(),
			//		coll._gameObject.getComponent<C_Transform2d>().transform.origin), _color);
			//
			//	debugDraw->Flush(main_camera);
			//});
			

		}
	}

	void _System_Physics::terminate() {

	}


}