#include "GravityForce.h"

void GravityForce::updateForce(Particle* particle, float t) {

	if (!getActive() || !particle->hasFiniteMass())return;

	particle->addForce(GRAVITY*particle->getMass());
}
