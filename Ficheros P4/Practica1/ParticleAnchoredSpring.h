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

	inline void increaseK() { k++; }
	inline void decreaseK() { k--; }

	virtual ~ParticleAnchoredSpring() {}
};

