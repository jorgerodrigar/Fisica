#include "ParticleAnchoredSpring.h"

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor_, float k_, float restLength_):
	anchor(anchor_), k(k_), restLength(restLength_) {
	transform.p = *anchor;
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

void ParticleAnchoredSpring::updateForce(physx::PxRigidDynamic* obj, float t) {
	if (getActive()) {
		Vector3 f = obj->getGlobalPose().p;
		f -= *anchor;

		float length = f.normalize();

		f *= -(length*k);
		obj->addForce(f);
	}
}

void ParticleAnchoredSpring::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case '+':
		increaseK();
		break;
	case '-':
		decreaseK();
		break;
	default:
		break;
	}
}
