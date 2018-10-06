#include "core.hpp"
#include "RenderUtils.hpp"

#pragma once
class Particle: public RenderItem            // clase particula, hereda de RenderItem para poder dibujarse
{
private:
	Vector3 p;                               // posicion
	float v;                                 // velocidad 
	Vector3 velocidad;
	Vector3 dir;
	Vector3 a;                               // aceleracion
	float damping;                           // rozamiento
	int inverse_mass;                        // masa inversa. Si es 0 la masa sera infinita
	physx::PxTransform transform;    
	// transform que contiene su posicion y orientacion actuales

	void integrate(float t);                 // actualiza los parametros de particula
public:
	Particle(physx::PxShape* _shape, Vector3 p_ = { 0.0, 0.0, 0.0 }, float vel = 0, Vector3 dir_ = { 1.0, 0.0, 0.0 },
		Vector3 a_ = { 0.0, 0.0, 0.0 }, float damping_ = 0.95, int inverse_mass_ = 1);

	void update(float t) { integrate(t); }

	// setters
	inline void setPosition(float x, float y, float z) { p = { x, y, z }; }
	inline void setVelocity(float vel) { v = vel; velocidad = v * dir; }
	inline void setDirection(float x, float y, float z) { dir = { x, y, z }; velocidad = v * dir; }
	inline void setAcceleration(float x, float y, float z) { a = { x, y, z }; }
	inline void setPosition(Vector3 pos) { p = pos; }
	inline void setDirection(Vector3 d) { dir = d; velocidad = v * dir; }
	inline void setAcceleration(Vector3 acc) { a = acc; }
	inline void setDamping(float damp) { damping = damp; }
	inline void setMass(int mass) { inverse_mass = 1 / mass; }

	~Particle() {}
};

