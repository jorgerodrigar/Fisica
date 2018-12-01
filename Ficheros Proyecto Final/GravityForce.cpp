#include "GravityForce.h"

void GravityForce::updateForce(Particle* particle, float t) {

	if (!getActive() || !particle->hasFiniteMass())return;

	particle->addForce(GRAVITY*particle->getMass());
}

void GravityForce::updateForce(physx::PxRigidDynamic* obj, float t) {

	if (!getActive())return;

	obj->addForce(GRAVITY*obj->getMass());
}
