#pragma once
#include "ParticleForceGenerator.h"

template<typename T>

class ParticleForceRegistry  // registro de todos los elementos con los generadores que actuan sobre ellos
{
protected:
	// almacena un elemento y el generador de fuerzas que le afecta
	struct ParticleForceRegistration {
		T* particle;
		ParticleForceGenerator* fg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations; // registro de todos los elementos con sus respectivos generadores de fuerza

public:
	ParticleForceRegistry() {}

	// añade un nuevo registro
	void add(T* particle, ParticleForceGenerator* fg) {     
		registrations.push_back({ particle, fg });
	}

	// elimina un registro
	void remove(T* particle, ParticleForceGenerator* fg) {  
		auto it = registrations.begin();
		while (it != registrations.end() && (it->particle != particle || it->fg != fg))it++;

		if (it != registrations.end())registrations.erase(it);
	}

	// elimina todos los resgistros (pero no las particulas ni los generadores)
	void clear() {                                          
		for (int i = 0; i < registrations.size(); i++)
			registrations.pop_back();
	} 

	// actualiza todos los generadores del registro
	void updateForces(float t) {                            
		for (int i = 0; i < registrations.size(); i++) {
			registrations[i].fg->updateForce(registrations[i].particle, t);
		}
	}

	~ParticleForceRegistry() { clear(); }
};

