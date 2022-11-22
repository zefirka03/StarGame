#pragma once
#include "air.h"

#include "S_World.h"

using namespace air;

class S_PlayerController : public Script {
	friend class S_PlayerFeet;
public:
	void setWorld(S_WorldGeneration* _world);
	void setCamera(C_Camera2d* _camera);
	void OnCreate() override;
	void OnUpdate(float _deltaTime) override;
	void OnDestroy() override;

private:
	S_WorldGeneration* World = nullptr;
	C_Camera2d* Camera = nullptr;

	int onGround = 0;
	float t = 0;

	C_Sprite* sp;
	C_Transform2d* tr;
	C_Collider_Box2d* cl;
	C_RigidBody* rg;

	Entity ent_feet;
};



class S_PlayerFeet : public Script {
public:
	S_PlayerFeet(S_PlayerController* _control);

	void OnCreate() override;

	void OnUpdate(float _deltaTime) override;

	void OnCollisionEnter(C_RigidBody& _other) override;

	void OnCollisionEnd(C_RigidBody& _other) override;


private:
	C_Collider_Box2d* cl_feet;
	C_RigidBody* rg_feet;

	S_PlayerController* h_control;
};