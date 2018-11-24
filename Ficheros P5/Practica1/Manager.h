#pragma once
#include "ParticleForceRegistry.h"

class Manager  // gestor abstracto puro
{
protected:
	ParticleForceRegistry<Particle>* registry = nullptr;  // registro de particulas y sus fuerzas
	std::vector<ParticleForceGenerator*> forceGenerators; // almacenamiento de fuerzas aplicadas en el registro
public:
	Manager() {}

	virtual void update(float t) = 0;
	// establecen los registros
	virtual inline void setForcesRegistry(ParticleForceRegistry<Particle>* registry_) { registry = registry_; }
	// añade una nueva fuerza
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) { forceGenerators.push_back(generator); };
	// gestiona eventos de teclado
	virtual void handleEvent(unsigned char key) = 0;           

	virtual ~Manager() {}
};

