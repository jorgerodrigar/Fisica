#include "Obstacles.h"
#include <iostream>

Vector3 Obstacles::randomizePos(int i) {
	float x;
	if (lastPosition.x == 0) // si es el primer obstaculo que ponemos, lo ponemos a la distancia minima del jugador
		x = -(rand() % maxFromPlayer + minFromPlayer) + playerPos.x;
	else                     // si no, a la distancia minima del obstaculo anterior
		x = -(rand() % maxFromLastObstacle + minFromLastObstacle) + lastPosition.x;

	float z = 0;
	if (widthTypes[i] == 0) {
		z = rand() % 3 + 1;
		z *= widths[widthTypes[i]];
	}
	else z = groundWidth / 2;
	int signo = rand() % 2;
	if (signo == 1) {
		z = -z;
		z += widths[widthTypes[i]] / 2;
	}
	else z -= widths[widthTypes[i]] / 2;
	lastPosition = { x, posY, z };

	return lastPosition;
}

Obstacles::Obstacles(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numObstacles_, Vector3 pos_, float groundWidth_) :
	InfiniteObjectsManager(gScene_, gPhysics_, numObstacles_, pos_), groundWidth(groundWidth_)
{
	for (int i = 1; i < 4; i++) widths.push_back((groundWidth / 3)*i);

	for (int i = 0; i < numElems; i++) {
		float widthNum = rand() % 3;
		widthTypes.push_back(widthNum);
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, height, widths[widthNum]));
		physx::PxTransform transform(randomizePos(i));
		physx::PxRigidStatic* obstacle = gPhysics->createRigidStatic(transform);
		obstacle->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, obstacle, { 0, 1, 1, 1 });
		gScene->addActor(*obstacle);
		shape->release();
		elems.push_back(obstacle);
	}

	first = 0;
	last = numElems - 1;
}

// si el jugador pasa del obstaculo mas cercano, este se situa aleatoriamente delante de el, y asi sucesivamente
// da sensacion de obstaculos infinitos usando solo numElems obstaculos
void Obstacles::update(float t) {
	if (playerPos.x < elems[first]->getGlobalPose().p.x - 200) {
		Vector3 newPos = randomizePos(first);
		elems[first]->setGlobalPose({ newPos.x, newPos.y, newPos.z });
		last = first;
		first++;
		if (first >= numElems)first = 0;
	}
}

void Obstacles::resetParameters() {  // los obstaculos vuelven a colocarse aleatoriamente respecto a la posicion inicial
	lastPosition = { 0, 0, 0 };
	playerPos = pos;
	first = 0;
	last = numElems - 1;

	for (int i = 0; i < numElems; i++) {
		elems[i]->setGlobalPose(physx::PxTransform(randomizePos(i)));
	}
}


