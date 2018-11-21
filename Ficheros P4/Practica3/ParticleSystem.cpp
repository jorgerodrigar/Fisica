#include "ParticleSystem.h"

void ParticleSystem::Shoot(float vel) {
	float x, y, z;
	int signoX, signoZ;
	x = rand() % 50 + 1;
	y = rand() % 400 + 1;
	z = rand() % 50 + 1;
	signoX = rand() % 2;
	signoZ = rand() % 2;
	if (signoX == 0)x = -x;
	if (signoZ == 0)z = -z;
	pool.Shoot(pos, { x, y, z });
	pool.setVel(vel);
}