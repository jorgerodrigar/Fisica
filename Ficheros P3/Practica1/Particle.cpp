#include "Particle.h"

// la constructora de RenderItem recibe una forma, un transform que engloba una posicion y una orientacion
// y un Vector3 para el color

Particle::Particle(RenderItem* renderItem_, Vector3 p_, Vector3 v_, Vector3 a_, float damping_, int inverse_mass_, float maxRec) :
	renderItem(renderItem_), p(p_), posIni(p_), v(v_), a(a_), damping(damping_), inverse_mass(inverse_mass_), 
	transform(p), maxRecorrido(maxRec), force({ 0, 0, 0 })
{
	renderItem->addReference();          // le suma 1 a las referencias que apuntan a renderItem (como un smartPtr)
	renderItem->transform = &transform;
}

void Particle::integrate(float t)       // actualiza los parametros de particula
{
	if (!active)return;                 // si la particula no esta activa no hace nada
	if (inverse_mass <= 0.0f) return;   // si la masa es infinita no hace nada

	p += v * t;                         // si no, actualizamos parametros y transform

	Vector3 totalAcceleration = a;
	a += force * inverse_mass;

	v += totalAcceleration * t;
	v *= powf(damping, t);

	clearForce();

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
	if (!active) DeregisterRenderItem(renderItem);  // si la hemos desactivado no se pinta
	else RegisterRenderItem(renderItem);            // si la hemos activado volvemos a pintarla
}
