#pragma once
#include "ParticleManager.h"

class ParticleSystem:public ParticleManager  // gestor del sistema de particulas
{
private:
	Vector3 pos;                             // posicion desde la que saldran las particulas
public:
	ParticleSystem(Vector3 pos_ = { 0.0, 0.0, 0.0 }):pos(pos_) {}

	void Shoot(float vel = 75);              // dispara particulas en direcciones aleatorias como una fuente

	virtual ~ParticleSystem() {}
};

