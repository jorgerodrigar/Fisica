#include "Particle.h"


// la constructora de RenderItem recibe una forma, un transform que engloba una posicion y una orientacion
// y un Vector3 para el color

Particle::Particle(physx::PxShape* _shape, Vector3 p_, float vel, Vector3 dir_, Vector3 a_, float damping_, int inverse_mass_) :
	p(p_), v(vel), dir(dir_), a(a_), damping(damping_), inverse_mass(inverse_mass_), transform(p), velocidad(v * dir),
	RenderItem(_shape, &transform, Vector3(1.0, 4.0, 3.0)) {}


void Particle::integrate(float t)       // actualiza los parametros de particula
{
	if (inverse_mass <= 0.0f) return;   // si la masa es infinita no hace nada

	p += velocidad * t;                         // si no, actualizamos parametros y transform
	velocidad += a * t;
	velocidad *= powf(damping, t);
	transform.p = p;
}
