#pragma once
#include "InfiniteObjectsManager.h"

class Obstacles : public InfiniteObjectsManager<physx::PxRigidStatic>  // gestiona los obstaculos
{
private:
	Vector3 lastPosition = { 0, 0, 0 }; // ultima posicion donde se coloco un obstaculo

	// posicion de los obstaculos
	const int minFromPlayer = 200;
	const int maxFromPlayer = 500;
	const int minFromLastObstacle = 50;
	const int maxFromLastObstacle = 200;
	const int maxLateral = 80;
	const float posY = -30;

	// dimensiones de los obstaculos
	float groundWidth;
	std::vector<int> widths;              // anchuras posibles
	const float length = 5;
	const float height = 5;

	std::vector<int> widthTypes;

	Vector3 randomizePos(int typeWidth);  // establece una posicion aleatoria

public:
	Obstacles(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numObstacles_, Vector3 pos_, float groundWidth_ = 75.0);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();

	virtual ~Obstacles() {}
};

