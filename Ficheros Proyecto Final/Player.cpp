#include "Player.h"

Player::Player(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos_, float tam, Vector4 color):
	GameObject(gScene_, gPhysics_, pos_)
{
	physx::PxShape* playerShape = CreateShape(physx::PxSphereGeometry(tam));
	player = gPhysics->createRigidDynamic(physx::PxTransform(pos));
	player->attachShape(*playerShape);
	RenderItem* playeRenderItem = new RenderItem(playerShape, player, color);
	physx::PxRigidBodyExt::updateMassAndInertia(*player, 1);
	gScene->addActor(*player);
	playerShape->release();
}

void Player::update(float t) {
	if (start)player->setLinearVelocity({ vel, player->getLinearVelocity().y, 0 });
}

void Player::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case 'A': {  // si se pulsa A, se mueve hacia la izquierda (si puede)
		if (actualPosition > LEFT) {
			Vector3 pos = player->getGlobalPose().p;
			pos.z += lateralMovement;
			physx::PxTransform trans(pos);
			player->setGlobalPose(trans);
			actualPosition = PlayerPosition(actualPosition - 1);
		}
		break;
	}
	case 'D': {  // lo mismo con D, pero hacia la derecha
		if (actualPosition < RIGHT) {
			Vector3 pos = player->getGlobalPose().p;
			pos.z -= lateralMovement;
			physx::PxTransform trans(pos);
			player->setGlobalPose(trans);
			actualPosition = PlayerPosition(actualPosition + 1);
		}
		break;
	}
	case ' ': {  // si se pulsa espacio y se puede saltar, salta
		if (jump) {
			player->setLinearVelocity({ player->getLinearVelocity().x, 
				jumpVelocity, player->getLinearVelocity().z });
			jump = false;
		}
		break;
	}
	default:
		break;
	}
}

void Player::resetParameters() {  // el jugador vuelve a su posicion y velocidad iniciales
	start = false;
	actualPosition = MIDDLE;
	player->setLinearVelocity({ 0, 0, 0 });
	player->setAngularVelocity({ 0, 0, 0 });
	player->setGlobalPose(physx::PxTransform(pos));
}
