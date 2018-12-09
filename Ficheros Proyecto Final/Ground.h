#pragma once
#include "InfiniteObjectsManager.h"

class Ground : public InfiniteObjectsManager<physx::PxRigidStatic> // representa el suelo sobre el que va el jugador
{                                             // esta dividido en un numero de trozos de suelo
private:
	float width;                              // dimensiones de cada trozo
	float length;

public:
	Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, 
		Vector3 pos_, float width_ = 75.0, float length_ = 300.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();

	virtual ~Ground() {}
};

