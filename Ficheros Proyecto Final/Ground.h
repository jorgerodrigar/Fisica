#pragma once
#include "RigidObject.h"
#include <vector>

class Ground : public RigidObject  // representa el suelo sobre el que va el jugador
{                                  // esta dividido en un numero de trozos de suelo
private:
	Vector3 pos;                   // posicion donde comienza el suelo (primer trozo)
	Vector3 playerPos;             // posicion del jugador (habra que ir actualizandola)
	int first, last;               // direcciones del primer y ultimo trozo en el vector

	float width;                   // dimensiones de cada trozo
	float length;
	int numGrounds;                // numero de trozos

	// vector con todos los trozos de suelo
	std::vector<physx::PxRigidStatic*> grounds;

public:
	Ground(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, 
		Vector3 pos_ = { 0, -35, 0 }, float width_ = 75.0, float length_ = 300.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}

	inline void setPlayerPos(Vector3 pos_) { playerPos = pos_; } // actualiza la posicion del jugador

	virtual ~Ground() {}
};

