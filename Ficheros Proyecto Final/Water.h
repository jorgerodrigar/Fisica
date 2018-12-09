#pragma once
#include "InfiniteObjectsManager.h"
#include "Particle.h"

class Water : public InfiniteObjectsManager<Particle> // representa el agua de fondo
{                                                     // esta dividido en un numero de trozos de agua
private:
	float width;                                      // dimensiones de cada trozo
	float length;

public:
	Water(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_,
		Vector3 pos_, float width_ = 225.0, float length_ = 300.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();

	virtual ~Water();
};

