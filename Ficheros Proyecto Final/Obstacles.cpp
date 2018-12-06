#include "Obstacles.h"
#include <iostream>

Vector3 Obstacles::randomizePos(int i) {
	float x;
	if(i==0)x = -(rand() % maxFromPlayer + minFromPlayer) + playerPos.x;
	else x = -(rand() % 200 + minFromLastObstacle) + obstacles[i - 1]->getGlobalPose().p.x;
	float z = rand() % maxLateral;
	int signo = rand() % 2;
	if (signo == 1)z = -z;
	
	return { x, posY, z };
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

		obstacles.push_back(obstacle); std::cout << obstacle->getGlobalPose().p.x << " ";
	}

	first = 0;
	last = numObstacles - 1;
}

// si el jugador pasa del obstaculo mas cercano, este se situa aleatoriamente delante de el, y asi sucesivamente
// da sensacion de obstaculos infinitos usando solo numObstacles obstaculos
void Obstacles::update(float t) {
	if (playerPos.x < obstacles[first]->getGlobalPose().p.x) {
		Vector3 newPos = randomizePos(first);
		obstacles[first]->setGlobalPose({ newPos.x, newPos.y, newPos.z });
		last = first;
		first++;
		if (first >= numObstacles)first = 0;
	}
}


