#include "RigidBodyManager.h"

void RigidBodyManager::createNewRigidBody() {
	if (currentRigidBodys < MAXRIGIDBODYS) {
		physx::PxShape* shape = CreateShape(physx::PxCapsuleGeometry(5, 7));
		physx::PxTransform transform(pos);
		physx::PxRigidDynamic* estatico = gPhysics->createRigidDynamic(transform);
		estatico->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, estatico, { 0, 1, 0, 1 });
		physx::PxRigidBodyExt::updateMassAndInertia(*estatico, 1);
		gScene->addActor(*estatico);
		shape->release();
		currentRigidBodys++;
	}
}

void RigidBodyManager::Shoot() {
	createNewRigidBody();
}

void RigidBodyManager::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case ' ':
	{
		createNewRigidBody();
		break;
	}
	default:
		break;
	}
}
