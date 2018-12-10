#pragma once
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include "RenderUtils.hpp"

class GameObject                          // objeto abstracto puro de RigidBodies (polimorfismo)
{
protected:
	physx::PxScene* gScene = nullptr;      // variables necesarias de main.cpp
	physx::PxPhysics* gPhysics = nullptr;

	Vector3 pos;                           // posicion inicial del objeto

public:
	GameObject(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos_):gScene(gScene_), gPhysics(gPhysics_), pos(pos_) {}

	// metodos abstractos que compartiran
	virtual void update(float t) = 0;
	virtual void handleEvent(unsigned char key) = 0;
	virtual void resetParameters() = 0;    // restablece parametros

	virtual ~GameObject() {}
};

