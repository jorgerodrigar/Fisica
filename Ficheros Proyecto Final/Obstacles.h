#pragma once
#include "RigidObject.h"

class Obstacles : public RigidObject  // gestiona los obstaculos
{
private:
	Vector3 playerPos = { 0, 0, 0 };  // posicion del jugador (habra que ir actualizandola)
	int first, last;                  // direcciones del primer y ultimo obstaculo en el vector

	// posicion de los obstaculos
	const int minFromPlayer = 200;
	const int maxFromPlayer = 1000;
	const int minFromLastObstacle = 100;
	const int maxLateral = 80;
	const float posY = -30;

	//dimensiones de los obstaculos
	const int minWidth = 5;          // anchura variable, lo demas siempre constante
	const int maxWidth = 50;
	const float length = 5;
	const float height = 5;

	int numObstacles;                 // numero de obstaculos

	// vector con todos los obstaculos
	std::vector<physx::PxRigidStatic*> obstacles;

	Vector3 randomizePos(int i);          // establece una posicion aleatoria

public:
	Obstacles(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numObstacles_);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}

	inline void setPlayerPos(Vector3 pos_) { playerPos = pos_; } // actualiza la posicion del jugador

	virtual ~Obstacles() {}
};

