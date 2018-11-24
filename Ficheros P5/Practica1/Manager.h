#pragma once
#include "ParticleForceRegistry.h"

class Manager  // gestor abstracto puro
{
protected:
	std::vector<ParticleForceGenerator*> forceGenerators; // almacenamiento de fuerzas aplicadas en el registro
public:
	Manager() {}

	virtual void update(float t) = 0;
	// añade una fuerza
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) { forceGenerators.push_back(generator); };
	// gestiona eventos de teclado
	virtual void handleEvent(unsigned char key) = 0;           

	virtual ~Manager() {}
};

