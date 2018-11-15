#pragma once
#include "ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator
{
private:
	Vector3* anchor;                  // posicion del extremo fijo del muelle
	float k;                          // constante de elasticidad
	float restLength;
	RenderItem* renderItem = nullptr; // representacion grafica de la posicion fija
	physx::PxTransform transform;     // transform donde se pintara renderItem

public:
	ParticleAnchoredSpring(Vector3* anchor_, float k_, float restLength_);

	virtual void updateForce(Particle* particle, float t);

	inline void increaseK() { k++; }
	inline void decreaseK() { k--; }

	virtual inline void setActive(bool act);

	virtual ~ParticleAnchoredSpring() { renderItem->release(); }
};

