#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
	registrations.push_back({ particle, fg });
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
	auto it = registrations.begin();
	while (it != registrations.end() && (it->particle != particle || it->fg != fg))it++;

	if (it != registrations.end())registrations.erase(it);
}

void ParticleForceRegistry::clear() {
	for (auto it = registrations.begin(); it != registrations.end(); it++)
		registrations.erase(it);
}

void ParticleForceRegistry::updateForces(float t) {
	for (int i = 0; i < registrations.size();i++) {
		registrations[i].fg->updateForce(registrations[i].particle, t);
	}
}


