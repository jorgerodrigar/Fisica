#pragma once
#include "RBManager.h"
#include <vector>

class Ground : public RBManager
{
private:
	Vector3 pos;
	Vector3* playerPos = nullptr;
	int first, last;

	float width;
	float length;
	int numGrounds;

	physx::PxScene* gScene = nullptr;      // variables necesarias para crearlos
	physx::PxPhysics* gPhysics = nullptr;

	std::vector<physx::PxRigidStatic*> grounds;

public:
	Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3* playerPos_, int numGrounds_, 
		Vector3 pos_ = { 0, -35, 0 }, float width_ = 75.0, float length_ = 300.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}

	virtual ~Ground() {}
};

