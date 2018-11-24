#pragma once
#include "Manager.h"

class RBManager: public Manager                                        // gestor abstracto puro para RigidBody
{
protected:
	ParticleForceRegistry<physx::PxRigidDynamic>* registry = nullptr;  // registro de RigidBody y sus fuerzas
public:
	RBManager() {}

	// establece los registros
	virtual inline void setForcesRegistry(ParticleForceRegistry<physx::PxRigidDynamic>* registry_) { registry = registry_; }

	virtual ~RBManager() {}
};

