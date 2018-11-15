#pragma once
#include "VisibleParticleForceGenerator.h"
#include "RenderUtils.hpp"

class ExplosionForce : public VisibleParticleForceGenerator
{
private:
	Vector3 posIni;                   // posicion inicial del area de la explosion
	float radio;
	Vector3 forceVec;                 // vector de la fuerza a aplicar
	const float FORCE;                // magintud de la fuerza a aplicar
	bool explota = false;

	// temporizadores de explosion
	const float timeExplosion;
	float last_time;
	float next_time;

public:
	ExplosionForce(float force = 20000, float radio_ = 10, Vector3 pos = { 0, 0, 0 }, float timeExplosion_ = 30);

	virtual void updateForce(Particle* particle, float t);

	void temporizador(float t);                             // calcula cuando puede volver a explotar tras haber explotado

	void aplicaFuerza(Particle* particle);                  // aplica la fuerza que corresponda a esa particula

	inline void Explota() { if(!explota) explota = true; }  // hace explotar (si puede)

	inline void setPosition(Vector3 pos) { transform.p = pos; }
	inline void setInitialPosition() { transform.p = posIni; }

	inline float getRadio() { return radio; }

	virtual ~ExplosionForce() {}
};

