#pragma once
#include "Manager.h"
#include "Pool.h"
#include "PxRigidDynamic.h"

class RigidBodyManager:public Manager      // gestor de RigidBody
{
private:
	Vector3 pos;                           // posicion de la cual salen
	float vel;                             // magnitud de la velocidad con la que salen
	physx::PxScene* gScene = nullptr;      // variables necesarias para crearlos
	physx::PxPhysics* gPhysics = nullptr;
	const int MAXRIGIDBODYS = 20;          // maximo de generaciones
	int currentRigidBodys = 0;             // numero de generaciones actuales

	void createNewRigidBody(Vector3 dir);  // crea un RigidBody en la posicion dada, con la vel y dir dadas

public:
	RigidBodyManager(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos_ = { 0.0, 0.0, 0.0 }, float vel_ = 60) : 
		gScene(gScene_), gPhysics(gPhysics_), pos(pos_), vel(vel_) {}

	virtual void update(float t) {}
	void Shoot();                                  // calcula una direccion aleatoria y manda crear un RigidBody con ella
	virtual void handleEvent(unsigned char key);   // gestor de input
	virtual inline void setForcesRegistry(ParticleForceRegistry<Particle>* registry_) {}               // establece un registro de particulas
	virtual inline void setForcesRegistry(ParticleForceRegistry<physx::PxRigidDynamic>* registry_) {}  // establece un registro de RigidBody
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) {  } // a�ade una fuerza

	virtual ~RigidBodyManager() {}
};

