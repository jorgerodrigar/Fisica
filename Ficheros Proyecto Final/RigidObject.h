#pragma once
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include "RenderUtils.hpp"

class RigidObject                          // objeto abstracto puro de RigidBodies (polimorfismo)
{
protected:
	physx::PxScene* gScene = nullptr;      // variables necesarias de main.cpp
	physx::PxPhysics* gPhysics = nullptr;

public:
	RigidObject(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_):gScene(gScene_), gPhysics(gPhysics_) {}

	// metodos abstractos que compartiran
	virtual void update(float t) = 0;
	virtual void handleEvent(unsigned char key) = 0;

	virtual ~RigidObject() {}
};

