#include "WaterBoxes.h"
#include <iostream>

Vector3 WaterBoxes::randomizePos() {
	float x = -(rand() % maxFromLastObstacle + minFromLastObstacle) + lastPosition.x;
	float z = rand() % maxLateral + minLateral;
	int signo = rand() % 2;
	if (signo == 1)z = -z;
	lastPosition = { x, posY, z };

	return lastPosition;
}

WaterBoxes::WaterBoxes(int numObstacles_, Vector3 pos_) : InfiniteObjectsManager(numObstacles_, pos_)
{
	for (int i = 0; i < numElems; i++) {
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, height, width));
		RenderItem* renderItem = new RenderItem(shape, { 0, 1, 1, 1 });
		Particle* box = new Particle(renderItem, randomizePos()); 
		box->setDamping(0.3);
		shape->release();

		elems.push_back(box);
	}

	first = 0;
	last = numElems - 1;
}

// si el jugador pasa de la caja mas cercana, esta se situa aleatoriamente delante de el, y asi sucesivamente
// da sensacion de cajas infinitas usando solo numElems cajas
void WaterBoxes::update(float t) {
	if (registry != nullptr)registry->updateForces(t);
	for (auto e : elems)e->update(t);

	if (playerPos.x < elems[first]->getPosition().x - 200) {
		elems[first]->setPosition(randomizePos());
		last = first;
		first++;
		if (first >= numElems)first = 0;
	}
}

void WaterBoxes::resetParameters() {  // las cajas vuelven a colocarse aleatoriamente respecto a la posicion inicial
	lastPosition = { 0, 0, 0 };
	playerPos = pos;
	first = 0;
	last = numElems - 1;

	for (int i = 0; i < numElems; i++) {
		elems[i]->setPosition(randomizePos());
	}
}

void WaterBoxes::createForcesRegistry() {
	PManager::createForcesRegistry();

	for (int i = 0; i < numElems; i++) {
		for (int j = 0; j < forceGenerators.size(); j++)
			registry->add(elems[i], forceGenerators[j]);
	}
}

WaterBoxes::~WaterBoxes() {
	for (auto e : elems) {
		delete e;
		e = nullptr;
	}
}


