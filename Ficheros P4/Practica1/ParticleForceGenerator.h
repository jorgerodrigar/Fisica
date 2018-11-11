#pragma once
#include "Particle.h"

// clase abstracta de la que heredaran todos los generadores de fuerzas
class ParticleForceGenerator
{
public:
	ParticleForceGenerator() {}
	virtual void updateForce(Particle* particle, float t) = 0;
	virtual ~ParticleForceGenerator() {}
};

