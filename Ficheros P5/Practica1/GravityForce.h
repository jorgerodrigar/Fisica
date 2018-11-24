#pragma once
#include "ParticleForceGenerator.h"

class GravityForce : public ParticleForceGenerator
{
private:
	const Vector3 GRAVITY;   // aceleracion de la gravedad
public:
	GravityForce(const Vector3& gravity) : GRAVITY(gravity) {}

	virtual void updateForce(Particle* particle, float t);

	virtual void handleEvent(unsigned char key) {}

	virtual ~GravityForce() {}
};

