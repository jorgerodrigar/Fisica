#pragma once
#include "Particle.h"

// clase abstracta de la que heredaran todos los generadores de fuerzas
class ParticleForceGenerator
{
protected:
	bool active = true;
public:
	ParticleForceGenerator() {}
	virtual void updateForce(Particle* particle, float t) = 0;
	virtual void handleEvent(unsigned char key) = 0;
	virtual inline void setActive(bool act) { active = act; }
	inline bool getActive() { return active; }
	virtual ~ParticleForceGenerator() {}
};

