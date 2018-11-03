#include "WindForce.h"

WindForce::WindForce(Vector3 force, float radio_, Vector3 pos) : FORCE(force), radio(radio_), transform(pos) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radio)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void WindForce::updateForce(Particle* particle, float t) {
	if (!particle->hasFiniteMass())return;

	// a las particulas que esten en su zona de accion se les aplicara la fuerza
	if ((particle->getPosition() - transform.p).magnitude() < radio)
		particle->addForce(FORCE*particle->getMass());
}
