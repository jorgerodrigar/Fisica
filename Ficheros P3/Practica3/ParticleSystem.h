#pragma once
#include "Particle.h"
#include "Pool.h"

class ParticleSystem                         // gestor del sistema de particulas
{
private:
	Pool<Particle> pool;
public:
	ParticleSystem() {}

	void shoot(float vel = 75);              // dispara particulas en direcciones aleatorias como una fuente

	void update(float t) { pool.Update(t); } // actualiza la pool

	inline void setForcesRegistry(ParticleForceRegistry* registry_) { pool.setForcesRegistry(registry_); } // establece un registro
	inline void addForceGenrator(ParticleForceGenerator* generator) { pool.addForceGenrator(generator); }  // añade una fuerza

	~ParticleSystem() {}
};

