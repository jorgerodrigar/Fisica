#include "ParticleSpring.h"

void ParticleSpring::updateForce(Particle* particle, float t) {
	Vector3 f = particle->getPosition();
	f -= other->getPosition();

	float length = f.normalize();

	f *= -((length - restLength)*k);
	particle->addForce(f);
}

void ParticleSpring::handleEvent(unsigned char key) {
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
