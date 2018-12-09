#include "Ground.h"

Ground::Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, Vector3 pos_, float width_, float length_):
	InfiniteObjectsManager(gScene_, gPhysics_, numGrounds_, pos_), width(width_), length(length_)
{
	// creo numElems 'trozos' de suelo, uno detras de otro y los guardo en el vector
	for (int i = 0; i < numElems; i++) {  
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, 0.1, width));
		physx::PxTransform transform({ pos.x - length * i, pos.y, pos.z });
		physx::PxRigidStatic* suelo = gPhysics->createRigidStatic(transform);
		suelo->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, suelo, { 0, 1, 0, 1 });
		gScene->addActor(*suelo);
		shape->release();

		elems.push_back(suelo);
	}

	// direcciones del primer y el ultimo 'trozo'
	first = 0;
	last = numElems - 1;
}

// si el jugador pasa del 'trozo' mas cercano, este se situa detras del ultimo, y asi sucesivamente
// da sensacion de suelo infinito usando solo numElems 'trozos'
void Ground::update(float t) {
	if (playerPos.x < elems[first + 1]->getGlobalPose().p.x) {
		elems[first]->setGlobalPose({ elems[last]->getGlobalPose().p.x - length, elems[last]->getGlobalPose().p.y, elems[last]->getGlobalPose().p.z});
		last = first;
		first++;
		if (first >= numElems - 1)first = 0;
	}
}

void Ground::resetParameters() {  // los 'trozos' de suelo vuelven a su posicion inicial
	first = 0;
	last = numElems - 1;

	for (int i = 0; i < numElems; i++) {
		elems[i]->setGlobalPose(physx::PxTransform({ pos.x - length * i, pos.y, pos.z }));
	}
}