#pragma once
#include "ParticleForceGenerator.h"

class ParticleBuoyancy : public ParticleForceGenerator
{
private:
	float maxDepth;
	float volume;
	float waterHeight;
	float liquidDensity;

	Vector3 waterLogic(float depth);

public:
	ParticleBuoyancy(float maxDepth_, float volume_, float waterHeight_, float liquidDensity_ = 100.0f) :
		maxDepth(maxDepth_), volume(volume_), waterHeight(waterHeight_), liquidDensity(liquidDensity_) {}

	virtual void updateForce(Particle* particle, float t);
	virtual void updateForce(physx::PxRigidDynamic* obj, float t);
	virtual void handleEvent(unsigned char key) {}

	virtual ~ParticleBuoyancy() {}
};

