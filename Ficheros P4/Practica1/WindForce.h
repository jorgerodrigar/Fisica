#pragma once
#include "ParticleForceGenerator.h"
#include "RenderUtils.hpp"

class WindForce : public ParticleForceGenerator
{
private:
	RenderItem* renderItem = nullptr; // representacion grafica de la zona de accion que tendra el viento
	physx::PxTransform transform;
	float radio;
	const Vector3 FORCE;
public:
	WindForce(Vector3 force, float radio_ = 10, Vector3 pos = { 0, 0, 0 });

	virtual void updateForce(Particle* particle, float t);

	virtual ~WindForce() { renderItem->release(); }
};

