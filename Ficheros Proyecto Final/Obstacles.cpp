#include "Obstacles.h"
#include <iostream>

Vector3 Obstacles::randomizePos(int i) {
	float x;
	if (lastPosition.x == 0) // si es el primer obstaculo que ponemos, lo ponemos a la distancia minima del jugador
		x = -(rand() % maxFromPlayer + minFromPlayer) + playerPos.x;
	else                     // si no, a la distancia minima del obstaculo anterior
		x = -(rand() % maxFromLastObstacle + minFromLastObstacle) + lastPosition.x;

	float z = rand() % maxLateral;
	int signo = rand() % 2;
	if (signo == 1)z = -z;
	lastPosition = { x, posY, z };

	return lastPosition;
}

Obstacles::Obstacles(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numObstacles_) :
	RigidObject(gScene_, gPhysics_), numObstacles(numObstacles_)
{
	for (int i = 0; i < numObstacles; i++) {
		float width = rand() % maxWidth + minWidth;
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, height, width));
		physx::PxTransform transform(randomizePos(i));
		physx::PxRigidStatic* obstacle = gPhysics->createRigidStatic(transform);
		obstacle->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, obstacle, { 0, 1, 1, 1 });
		gScene->addActor(*obstacle);
		shape->release();

		obstacles.push_back(obstacle);
	}

	first = 0;
	last = numObstacles - 1;
}

// si el jugador pasa del obstaculo mas cercano, este se situa aleatoriamente delante de el, y asi sucesivamente
// da sensacion de obstaculos infinitos usando solo numObstacles obstaculos
void Obstacles::update(float t) {
	if (playerPos.x < obstacles[first]->getGlobalPose().p.x - 200) {
		Vector3 newPos = randomizePos(first);
		obstacles[first]->setGlobalPose({ newPos.x, newPos.y, newPos.z });
		last = first;
		first++;
		if (first >= numObstacles)first = 0;
	}
}


