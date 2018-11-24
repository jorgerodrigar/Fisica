#include "RigidBodyManager.h"

void RigidBodyManager::createNewRigidBody(Vector3 dir) {
		physx::PxShape* shape = CreateShape(physx::PxCapsuleGeometry(5, 7));
		physx::PxTransform transform(pos);
		physx::PxRigidDynamic* obj = gPhysics->createRigidDynamic(transform);
		obj->attachShape(*shape);
		RenderItem* renderItem = new RenderItem(shape, obj, { 1, 0, 0, 1 });
		physx::PxRigidBodyExt::updateMassAndInertia(*obj, 10);
		obj->setLinearVelocity(dir*vel);
		gScene->addActor(*obj);
		shape->release();
		currentRigidBodys++;
}

void RigidBodyManager::Shoot() {
	if (currentRigidBodys < MAXRIGIDBODYS) {
		float x, y, z;
		int signoX, signoZ;
		x = rand() % 50 + 1;
		y = rand() % 400 + 1;
		z = rand() % 50 + 1;
		signoX = rand() % 2;
		signoZ = rand() % 2;
		if (signoX == 0)x = -x;
		if (signoZ == 0)z = -z;
		Vector3 dir = { x, y, z };
		dir.normalize();
		createNewRigidBody(dir);
	}
}

void RigidBodyManager::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case ' ':
	{
		Shoot();
		break;
	}
	default:
		break;
	}
}
