#pragma once
#include "ParticleForceGenerator.h"
#include "Particle.h"

class ParticleSpring : public ParticleForceGenerator
{
private:
	Particle* other;
	float k;
	float restLength;

public:
	ParticleSpring(Particle* other_, float k_, float restLength_) :
		other(other_), k(k_), restLength(restLength_) {}

	virtual void updateForce(Particle* particle, float t);
	virtual void updateForce(physx::PxRigidDynamic* obj, float t);
	virtual void handleEvent(unsigned char key);

	inline void increaseK() { if (k < 100) k += 10; }
	inline void decreaseK() { if (k > 10) k -= 10; }

	virtual ~ParticleSpring() {}
};

