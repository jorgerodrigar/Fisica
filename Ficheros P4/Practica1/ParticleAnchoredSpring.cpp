#include "ParticleAnchoredSpring.h"



ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor_, float k_, float restLength_):
anchor(anchor_), k(k_), restLength(restLength_), transform(*anchor) {
	renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(5, 5, 5)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float t) {
	if (getActive()) {
		Vector3 f = particle->getPosition();
		f -= *anchor;

		float length = f.normalize();

		f *= -(length*k);
		particle->addForce(f);
	}
}

void ParticleAnchoredSpring::setActive(bool act) {
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
