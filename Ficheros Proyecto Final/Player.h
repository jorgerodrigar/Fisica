#pragma once
#include "RigidObject.h"

enum PlayerPosition {
	LEFT,
	MIDDLE,
	RIGHT
};

class Player:public RigidObject
{
private:
	physx::PxRigidDynamic* player = nullptr;

	PlayerPosition actualPosition = MIDDLE;
	bool jump = true;

public:
	Player(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos = {0, -34, 0}, float tam = 5, Vector4 color = { 1, 0, 0, 1 });

	virtual void update(float t);

	virtual void handleEvent(unsigned char key);

	physx::PxRigidDynamic* getObject() { return player; }
	void setLinearVelocity(Vector3 vel_){ player->setLinearVelocity(vel_); }

	virtual ~Player() {}
};

