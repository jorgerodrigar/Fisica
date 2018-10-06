#include "Particle.h"


// la constructora de RenderItem recibe una forma, un transform que engloba una posicion y una orientacion
// y un Vector3 para el color

Particle::Particle(physx::PxShape* _shape, Vector3 p_, Vector3 v_, Vector3 a_, float damping_, int inverse_mass_, float maxRec) :
	p(p_), posIni(p_), v(v_), a(a_), damping(damping_), inverse_mass(inverse_mass_), transform(p), maxRecorrido(maxRec),
	RenderItem(_shape, &transform, Vector3(1.0, 4.0, 3.0)) {}


void Particle::integrate(float t)       // actualiza los parametros de particula
{
	if (!active)return;                 // si la particula no esta activa no hace nada
	if (inverse_mass <= 0.0f) return;   // si la masa es infinita no hace nada

	p += v * t;                         // si no, actualizamos parametros y transform
	v += a * t;
	v *= powf(damping, t);
	transform.p = p;
	if ((p - posIni).magnitude() > maxRecorrido)setActive(false); // al recorrer una distancia, la particula se desactiva
}

inline void Particle::setVelocityDirection(float x, float y, float z) 
{ 
	Vector3 dir = { x, y, z }; 
	v = dir * getVelocityMagnitude(); 
}

inline void Particle::setAccelerationDirection(float x, float y, float z) 
{ 
	Vector3 dir = { x, y, z }; 
	a = dir * getAccelerationMagnitude(); 
}

inline void Particle::setActive(bool act) 
{ 
	active = act; 
	if (!active) DeregisterRenderItem(this);  // si la hemos desactivado no se pinta
	else RegisterRenderItem(this);            // si la hemos activado volvemos a pintarla
}
