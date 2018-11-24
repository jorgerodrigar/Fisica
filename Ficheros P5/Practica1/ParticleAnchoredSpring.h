#pragma once
#include "VisibleParticleForceGenerator.h"

class ParticleAnchoredSpring : public VisibleParticleForceGenerator
{
private:
	Vector3* anchor;                  // posicion del extremo fijo del muelle
	float k;                          // constante de elasticidad
	float restLength;

public:
	ParticleAnchoredSpring(Vector3* anchor_, float k_, float restLength_);

	virtual void updateForce(Particle* particle, float t);
	virtual void updateForce(physx::PxRigidDynamic* obj, float t);

	virtual void handleEvent(unsigned char key);

	inline void increaseK() { if(k < 100) k+=10; }
	inline void decreaseK() { if(k > 10) k-=10; }

	virtual ~ParticleAnchoredSpring() {}
};

