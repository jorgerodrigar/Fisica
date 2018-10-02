#include "core.hpp"
#include "RenderUtils.hpp"

#pragma once
class Particle : public RenderItem            // clase particula, hereda de RenderItem para poder dibujarse
{
private:
	Vector3 p;                               // posicion
	Vector3 v;                               // velocidad  
	Vector3 a;                               // aceleracion
	float damping;                           // rozamiento
	int inverse_mass;                        // masa inversa. Si es 0 la masa sera infinita
	physx::PxTransform transform;            // transform que contiene su posicion y orientacion actuales

	void integrate(float t)                 // actualiza los parametros de particula
	{
		if (inverse_mass <= 0.0f) return;   // si la masa es infinita no hace nada

		p += v * t;                         // si no, actualizamos parametros y transform
		v += a * t;
		v *= powf(damping, t);
		transform.p = p;
	}
public:
	Particle(physx::PxShape* _shape, Vector3 p_ = { 0.0, 0.0, 0.0 }, Vector3 v_ = { 0.0, 0.0, 0.0 },
		Vector3 a_ = { 0.0, 0.0, 0.0 }, float damping_ = 1.0, int inverse_mass_ = 0) :
		p(p_), v(v_), a(a_), damping(damping_), inverse_mass(inverse_mass_), transform(p),
		RenderItem(_shape, &transform, Vector3(3.0, 3.0, 3.0)) {}
	// la constructora de RenderItem recibe una forma, un transform que engloba una posicion y una orientacion
	// y un Vector3 para el color

	void update(float t) { integrate(t); }

	// setters
	void setPosition(float x, float y, float z) { p = { x, y, z }; }
	void setVelocity(float x, float y, float z) { v = { x, y, z }; }
	void setAcceleration(float x, float y, float z) { a = { x, y, z }; }
	void setDamping(float damp) { damping = damp; }
	void setMass(int mass) { inverse_mass = 1 / mass; }

	~Particle() {}
};

