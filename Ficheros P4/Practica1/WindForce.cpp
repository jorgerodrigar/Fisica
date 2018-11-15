#include "WindForce.h"

WindForce::WindForce(Vector3 force, float radio_, Vector3 pos) : FORCE(force), radio(radio_), transform(pos) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radio)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void WindForce::updateForce(Particle* particle, float t) {
	if (getActive()) {
		if (!particle->hasFiniteMass())return;

		// a las particulas que esten en su zona de accion se les aplicara la fuerza
		if ((particle->getPosition() - transform.p).magnitude() < radio)
			particle->addForce(FORCE*particle->getMass());
	}
}

void WindForce::setActive(bool act) {
	ParticleForceGenerator::setActive(act);
	if (!active && registered) {     // si la hemos desactivado y esta registrada no se pinta
		DeregisterRenderItem(renderItem);
		registered = false;
	}
	else if (active && !registered) { // si la hemos activado y no esta registrada volvemos a pintarla
		RegisterRenderItem(renderItem);
		registered = true;
	}
}
