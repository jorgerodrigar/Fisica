#include "Ground.h"

Ground::Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3* playerPos_, int numGrounds_, Vector3 pos_, float width_, float length_):
	gScene(gScene_), gPhysics(gPhysics_), playerPos(playerPos_), numGrounds(numGrounds_), pos(pos_), width(width_), length(length_)
{
	for (int i = 0; i < numGrounds; i++) {
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(300, 0.1, 75));
		physx::PxTransform transform({ pos.x - length * i, pos.y, pos.z });
		physx::PxRigidStatic* suelo = gPhysics->createRigidStatic(transform);
		suelo->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, suelo, { 0, 1, 0, 1 });
		gScene->addActor(*suelo);
		shape->release();

		grounds.push_back(suelo);
	}

	first = 0;
	last = numGrounds - 1;
}

void Ground::update(float t) {
	if (playerPos->x > grounds[last]->getGlobalPose().p.x - 10) {
		grounds[first]->setGlobalPose({ grounds[last]->getGlobalPose().p.x - length, grounds[last]->getGlobalPose().p.y, grounds[last]->getGlobalPose().p.z});
		last = first;
		first++;
		if (first >= numGrounds)first = 0;
	}
}
