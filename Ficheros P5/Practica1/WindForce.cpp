#include "WindForce.h"

WindForce::WindForce(Vector3 force, float radio_, Vector3 pos) : 
	VisibleParticleForceGenerator::VisibleParticleForceGenerator(pos), FORCE(force), radio(radio_) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radio)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void WindForce::updateForce(Particle* particle, float t) {

	if (!getActive() || !particle->hasFiniteMass())return;

	// a las particulas que esten en su zona de accion se les aplicara la fuerza
	if ((particle->getPosition() - transform.p).magnitude() < radio)
		particle->addForce(FORCE*particle->getMass());
}

void WindForce::updateForce(physx::PxRigidDynamic* obj, float t) {
	if ((obj->getGlobalPose().p - transform.p).magnitude() < radio)
		obj->addForce(FORCE*obj->getMass());
}

void WindForce::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case ' ':
		active = !active;
		setActive(active);
		break;
	default:
		break;
	}
}
