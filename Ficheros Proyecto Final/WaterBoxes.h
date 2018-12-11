#pragma once
#include "InfiniteObjectsManager.h"
#include "PManager.h"

class WaterBoxes : public InfiniteObjectsManager<Particle>, public PManager  // gestiona las cajas flotantes
{
private:
	Vector3 lastPosition = { 0, 0, 0 }; // ultima posicion donde se coloco una caja

	// posicion de las cajas
	const int minFromLastObstacle = 50;
	const int maxFromLastObstacle = 200;
	const int maxLateral = 120;
	const int minLateral = 75;
	const float posY = -56;

	//dimensiones de las cajas
	const int width = 5;
	const float length = 5;
	const float height = 5;

	Vector3 randomizePos();       // establece una posicion aleatoria

public:
	WaterBoxes(int numObstacles_, Vector3 pos_);

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();
	virtual void createForcesRegistry();

	virtual ~WaterBoxes();
};

