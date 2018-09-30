#include "core.hpp"
#include "RenderUtils.hpp"

#pragma once
class Particle: public RenderItem      //clase particula, puede dibujarse
{
private:
	Vector3 p;         //posicion
	Vector3 v;         //velocidad  
	Vector3 a;         //aceleracion
	float damping;     //rozamiento
	int inverse_mass;  //masa inversa. Si es 0 la masa sera infinita

	void integrate(float t)    //actualiza los parametros de particula
	{
		if (inverse_mass <= 0.0f) return;   //si la masa es infinita no hace nada

		p += v * t;                         //si no, actualizamos parametros
		v += a * t;
		v *= powf(damping, t);
	}
public:
	Particle(physx::PxShape* _shape, Vector3 p_ = { 0.0, 0.0, 0.0 }, Vector3 v_ = { 0.0, 0.0, 0.0 }, 
		Vector3 a_ = { 0.0, 0.0, 0.0 }, float damping_ = 1.0, int inverse_mass_ = 0) :
		p(p_), v(v_), a(a_), damping(damping_), inverse_mass(inverse_mass_),
		RenderItem(_shape, &physx::PxTransform(p), Vector3(3.0, 3.0, 3.0)) {}
	//la constructora de RenderItem recibe una forma, un transform que engloba una posicion y una orientacion
	//y un Vector3 para el color

	void update(float t) { integrate(t); }

	void setPosition(Vector3 pos) { p = pos; }
	void setVelocity(Vector3 vel) { v = vel; }
	void setAcceleration(Vector3 acc) { a = acc; }
	void setDamping(float damp) { damping = damp; }
	void setMass(int mass) { inverse_mass = 1 / mass; }

	~Particle() {}
};

