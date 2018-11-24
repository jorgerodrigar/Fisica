#pragma once
#include "PManager.h"
#include "Pool.h"

class ParticleManager:public PManager  // gestor de particulas
{
protected:
	Pool<Particle> pool;

private:
	void shoot(Vector3 pos, Vector3 dir) { pool.Shoot(pos, dir); } // dispara particulas en la posicion y direccion dadas

public:
	ParticleManager() {}

	virtual void update(float t) { pool.Update(t); }               // actualiza la pool

	// establece un registro de particulas y se lo da a la Pool
	virtual inline void setForcesRegistry(ParticleForceRegistry<Particle>* registry_) { pool.setForcesRegistry(registry_); }
	// a�ade una fuerza al registro
	virtual inline void addForceGenrator(ParticleForceGenerator* generator_) { pool.addForceGenrator(generator_); }

	virtual void handleEvent(unsigned char key);

	~ParticleManager() {}
};

