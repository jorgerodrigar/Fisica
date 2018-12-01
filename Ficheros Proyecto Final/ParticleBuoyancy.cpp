#include "ParticleBuoyancy.h"

void ParticleBuoyancy::updateForce(Particle* particle, float t) {
	float depth = particle->getPosition().y;
	if (depth > (waterHeight + maxDepth))return; // si esta fuera del agua

	Vector3 f = waterLogic(depth);

	particle->addForce(f);
}

void ParticleBuoyancy::updateForce(physx::PxRigidDynamic* obj, float t) {
	float depth = obj->getGlobalPose().p.y;

	Vector3 f = waterLogic(depth);

	obj->addForce(f);
}

Vector3 ParticleBuoyancy::waterLogic(float depth) {
	Vector3 f(0.0, 0.0, 0.0);
	if (depth <= (waterHeight - maxDepth)) {
		f.y = liquidDensity * volume;          // si esta totalmente bajo el agua
	}
	else {                                     // si esta mitad fuera, mitad dentro
		float depthExterior = waterHeight + maxDepth;
		float volumeFactor = (depth - depthExterior) / (2 * maxDepth);
		f.y = liquidDensity * volumeFactor*volume;
	}

	return f;
}
