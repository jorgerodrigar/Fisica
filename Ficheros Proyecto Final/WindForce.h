#pragma once
#include "VisibleParticleForceGenerator.h"
#include "RenderUtils.hpp"

class WindForce : public VisibleParticleForceGenerator
{
private:
	float radio;
	const Vector3 FORCE;

public:
	WindForce(Vector3 force, float radio_ = 10, Vector3 pos = { 0, 0, 0 });

	virtual void updateForce(Particle* particle, float t);
	virtual void updateForce(physx::PxRigidDynamic* obj, float t);

	virtual void handleEvent(unsigned char key);

	virtual ~WindForce() {}
};

