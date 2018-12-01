#pragma once
#include "ParticleForceGenerator.h"

// generadores de fuerza con representacion grafica
class VisibleParticleForceGenerator : public ParticleForceGenerator 
{
protected:
	RenderItem* renderItem = nullptr; // representacion grafica del generador
	physx::PxTransform transform;     // transform donde se pintara renderItem
	bool registered = true;           // resgistrado en la lista de renderItems o no

public:
	VisibleParticleForceGenerator(Vector3 pos = { 0, 0, 0 }):transform(pos) {}
	virtual void setActive(bool act);
	virtual ~VisibleParticleForceGenerator() { renderItem->release(); }
};

