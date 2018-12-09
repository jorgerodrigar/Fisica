#pragma once
#include "InfiniteObjectsManager.h"
#include "Particle.h"
#include "ParticleSystem.h"

class Water : public InfiniteObjectsManager<Particle>, public PManager // representa el agua de fondo
{                                                     // esta dividido en un numero de trozos de agua
private:
	float width;                                      // dimensiones de cada trozo
	float length;

	float last_time = 0;
	float next_time = 0;
	const float timeShoot = 0.01;

	std::vector<std::pair<ParticleSystem*, ParticleSystem*>> waterFalls; // cascadas a lo largo del agua

public:
	Water(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_,
		Vector3 pos_, float width_ = 225.0, float length_ = 300.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();
	virtual void createForcesRegistry();

	virtual ~Water();
};

