#pragma once
#include "ParticleForceRegistry.h"

class Manager  // gestor abstracto puro
{
public:
	Manager() {}
	virtual void update(float t) = 0;
	virtual inline void setForcesRegistry(ParticleForceRegistry* registry_) = 0;  // establece un registro
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) = 0;  // añade una fuerza
	virtual void handleEvent(unsigned char key) = 0;                              // gestiona eventos de teclado
	virtual ~Manager() {}
};

