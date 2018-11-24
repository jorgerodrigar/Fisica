#pragma once
#include "Manager.h"

class PManager: public Manager                            // gestor abstracto puro para particulas
{
protected:
	ParticleForceRegistry<Particle>* registry = nullptr;  // registro de particulas y sus fuerzas
public:
	PManager() {}

	// establece los registros
	virtual inline void setForcesRegistry(ParticleForceRegistry<Particle>* registry_) { registry = registry_; }

	virtual ~PManager() {}
};

