#pragma once
#include "Manager.h"
#include "Pool.h"

class ParticleManager:public Manager  // gestor de particulas
{
protected:
	Pool<Particle> pool;

private:
	void shoot(Vector3 pos, Vector3 dir) { pool.Shoot(pos, dir); } // dispara particulas en la posicion y direccion dadas

public:
	ParticleManager() {}

	virtual void update(float t) { pool.Update(t); }               // actualiza la pool

	virtual inline void setForcesRegistry(ParticleForceRegistry* registry_) { pool.setForcesRegistry(registry_); } // establece un registro
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) { pool.addForceGenrator(generator); }  // añade una fuerza
	virtual void handleEvent(unsigned char key);

	~ParticleManager() {}
};

