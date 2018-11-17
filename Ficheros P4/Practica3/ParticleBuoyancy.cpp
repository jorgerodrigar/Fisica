#include "ParticleBuoyancy.h"

void ParticleBuoyancy::updateForce(Particle* particle, float t) {
	float depth = particle->getPosition().y;

	if (depth > (waterHeight + maxDepth))return; // si esta fuera del agua

	Vector3 f(0.0, 0.0, 0.0);
	if (depth <= (waterHeight - maxDepth)) {
		f.y = liquidDensity * volume;            // totalmente bajo el agua
	}
	else {                                       // mitad fuera mitad dentro
		float depthExterior = waterHeight + maxDepth;
		float volumeFactor = (depth - depthExterior) / (2 * maxDepth);
		f.y = liquidDensity * volumeFactor*volume;
	}
	particle->addForce(f);
}
