#include "Water.h"

Water::Water(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, Vector3 pos_, float width_, float length_) :
	InfiniteObjectsManager(gScene_, gPhysics_, numGrounds_, pos_), width(width_), length(length_)
{
	// creo numElems 'trozos' de agua, uno detras de otro y los guardo en el vector
	for (int i = 0; i < numElems; i++) {
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, 0.1, width));
		RenderItem* renderItem = new RenderItem(shape, { 0, 0, 1, 1 });
		Particle* agua = new Particle(renderItem, { pos.x - length * i, pos.y, pos.z });
		shape->release();

		elems.push_back(agua);
	}

	// direcciones del primer y el ultimo 'trozo'
	first = 0;
	last = numElems - 1;
}

// si el jugador pasa del 'trozo' mas cercano, este se situa detras del ultimo, y asi sucesivamente
// da sensacion de agua infinita usando solo numElems 'trozos'
void Water::update(float t) {
	for (auto e : elems)e->update(t);

	if (playerPos.x < elems[first + 1]->getPosition().x) {
		elems[first]->setPosition({ elems[last]->getPosition().x - length, elems[last]->getPosition().y, elems[last]->getPosition().z });
		last = first;
		first++;
		if (first >= numElems - 1)first = 0;
	}
}

void Water::resetParameters() {  // los 'trozos' de agua vuelven a su posicion inicial
	first = 0;
	last = numElems - 1;

	for (int i = 0; i < numElems; i++) {
		elems[i]->setPosition({ pos.x - length * i, pos.y, pos.z });
	}
}

Water::~Water() {
	for (auto e : elems) {
		delete e;
		e = nullptr;
	}
}