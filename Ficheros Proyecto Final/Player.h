#pragma once
#include "RigidObject.h"

// estados en los que puede encontrarse el jugador
enum PlayerPosition {
	LEFT,
	MIDDLE,
	RIGHT
};

class Player:public RigidObject              // representa al jugador
{
private:
	physx::PxRigidDynamic* player = nullptr;

	PlayerPosition actualPosition = MIDDLE;  // posicion inicial en el medio
	const float lateralMovement = 30;        // distancia lateral que recorre
	const float jumpVelocity = 75;           // velocidad de salto
	bool jump = true;

public:
	Player(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos = {0, -34, 0}, float tam = 5, Vector4 color = { 1, 0, 0, 1 });

	virtual void update(float t);

	virtual void handleEvent(unsigned char key);

	physx::PxRigidDynamic* getObject() { return player; }
	void setLinearVelocity(Vector3 vel_){ player->setLinearVelocity(vel_); }
	inline void playerCanJump() { jump = true; }

	virtual ~Player() {}
};

