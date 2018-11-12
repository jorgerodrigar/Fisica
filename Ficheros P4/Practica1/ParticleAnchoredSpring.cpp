#include "ParticleAnchoredSpring.h"



ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor_, float k_, float restLength_):
anchor(anchor_), k(k_), restLength(restLength_), transform(*anchor) {
	renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(5, 5, 5)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float t) {
	Vector3 f = particle->getPosition();
	f -= *anchor;

	float length = f.normalize();

	f *= -(length*k);
	particle->addForce(f);
}
