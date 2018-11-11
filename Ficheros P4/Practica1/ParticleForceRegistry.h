#pragma once
#include "ParticleForceGenerator.h"

class ParticleForceRegistry  // registro de todas las particulas con los generadores que actuan sobre ellas
{
protected:
	// almacena una particula y el generador de fuerzas que le afecta
	struct ParticleForceRegistration {
		Particle* particle;
		ParticleForceGenerator* fg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations; // registro de todas las particulas con sus respectivos generadores de fuerza

public:
	ParticleForceRegistry() {}

	void add(Particle* particle, ParticleForceGenerator* fg);    // añade un nuevo registro
	void remove(Particle* particle, ParticleForceGenerator* fg); // elimina un registro
	void clear();                                                // elimina todos los resgistros (pero no las particulas ni los generadores)
	void updateForces(float t);                                  // actualiza todos los generadores del registro

	~ParticleForceRegistry() { clear(); }
};

