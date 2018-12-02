#include "Player.h"

Player::Player(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos, float tam, Vector4 color):
	RigidObject(gScene_, gPhysics_)
{
	physx::PxShape* playerShape = CreateShape(physx::PxSphereGeometry(tam));
	physx::PxTransform playerTrans(pos);
	player = gPhysics->createRigidDynamic(playerTrans);
	player->attachShape(*playerShape);
	RenderItem* playeRenderItem = new RenderItem(playerShape, player, color);
	physx::PxRigidBodyExt::updateMassAndInertia(*player, 1);
	gScene->addActor(*player);
	playerShape->release();
}

void Player::update(float t) {
	player->setLinearDamping(0);
	player->setAngularDamping(0);

	//if (player->getLinearVelocity().y == 0)jump = true;
}

void Player::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case 'A': {
		if (actualPosition > LEFT) {
			Vector3 pos = player->getGlobalPose().p;
			pos.z += 25;
			physx::PxTransform trans(pos);
			player->setGlobalPose(trans);
			actualPosition = PlayerPosition(actualPosition - 1);
		}
		break;
	}
	case 'D': {
		if (actualPosition < RIGHT) {
			Vector3 pos = player->getGlobalPose().p;
			pos.z -= 25;
			physx::PxTransform trans(pos);
			player->setGlobalPose(trans);
			actualPosition = PlayerPosition(actualPosition + 1);
		}
		break;
	}
	case ' ': {
		//if (jump) {
			player->setLinearVelocity({ player->getLinearVelocity().x, 50, 0 });
			jump = false;
		//}
		break;
	}
	default:
		break;
	}
}
