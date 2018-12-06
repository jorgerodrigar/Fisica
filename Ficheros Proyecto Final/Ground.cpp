#include "Ground.h"

Ground::Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, Vector3 pos_, float width_, float length_):
	RigidObject(gScene_, gPhysics_), numGrounds(numGrounds_), pos(pos_), width(width_), length(length_)
{
	// creo numGrounds 'trozos' de suelo, uno detras de otro y los guardo en el vector
	for (int i = 0; i < numGrounds; i++) {  
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, 0.1, width));
		physx::PxTransform transform({ pos.x - length * i, pos.y, pos.z });
		physx::PxRigidStatic* suelo = gPhysics->createRigidStatic(transform);
		suelo->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, suelo, { 0, 1, 0, 1 });
		gScene->addActor(*suelo);
		shape->release();

		grounds.push_back(suelo);
	}

	// direcciones del primer y el ultimo 'trozo'
	first = 0;
	last = numGrounds - 1;
}

// si el jugador pasa del 'trozo' mas cercano, este se situa detras del ultimo, y asi sucesivamente
// da sensacion de suelo infinito usando solo numGround 'trozos'
void Ground::update(float t) {
	if (playerPos.x < grounds[first + 1]->getGlobalPose().p.x) {
		grounds[first]->setGlobalPose({ grounds[last]->getGlobalPose().p.x - length, grounds[last]->getGlobalPose().p.y, grounds[last]->getGlobalPose().p.z});
		last = first;
		first++;
		if (first >= numGrounds - 1)first = 0;
	}
}
