#pragma once
#include "Manager.h"
#include "Pool.h"
#include "PxRigidDynamic.h"

class RigidBodyManager:public Manager
{
private:
	Vector3 pos;
	physx::PxScene* gScene = nullptr;
	physx::PxPhysics* gPhysics = nullptr;
	const int MAXRIGIDBODYS = 10;
	int currentRigidBodys = 0;

	void createNewRigidBody();

public:
	RigidBodyManager(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos_ = { 0.0, 0.0, 0.0 }) : 
		gScene(gScene_), gPhysics(gPhysics_), pos(pos_) {}

	virtual void update(float t) {}
	void Shoot();
	virtual void handleEvent(unsigned char key);
	virtual inline void setForcesRegistry(ParticleForceRegistry* registry_) {  } // establece un registro
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) {  }  // añade una fuerza

	virtual ~RigidBodyManager() {}
};

