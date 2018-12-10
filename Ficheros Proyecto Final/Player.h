#pragma once
#include "GameObject.h"

// estados en los que puede encontrarse el jugador
enum PlayerPosition {
	LEFT,
	MIDDLE,
	RIGHT
};

class Player:public GameObject              // representa al jugador
{
private:
	physx::PxRigidDynamic* player = nullptr; // cuerpo del jugador

	// movimiento
	PlayerPosition actualPosition = MIDDLE;  // posicion inicial en el medio
	float vel = 0;                           // velocidad lineal de avance
	const float jumpVelocity = 75;           // velocidad lineal de salto
	const float lateralMovement = 30;        // distancia lateral que recorre

	// logica
	bool start = false;                      // indica si el juego ha empezado o no
	bool jump = true;                        // indica si puede saltar o no

public:
	Player(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, Vector3 pos_, float tam = 5, Vector4 color = { 1, 0, 0, 1 });

	virtual void update(float t);
	virtual void handleEvent(unsigned char key);
	virtual void resetParameters();

	physx::PxRigidDynamic* getObject() { return player; }
	inline void setVelocity(float vel_) { vel = vel_; }
	inline void playerCanJump() { jump = true; }
	inline void startRunning() { start = true; }

	virtual ~Player() {}
};

