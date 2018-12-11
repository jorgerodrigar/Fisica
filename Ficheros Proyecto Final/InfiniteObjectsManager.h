#pragma once
#include "GameObject.h"

template<typename T>
class InfiniteObjectsManager : public GameObject // gestor de objetos que parecen infinitos (suelo, agua, obstaculos, ...)
{
protected:
	Vector3 playerPos = { 0, 0, 0 };    // posicion actual del jugador (habra que ir actualizandola)
	int first, last;                    // direcciones del primer y ultimo elemento en el vector
	int numElems;                       // numero de elementos en los que estara dividido

	// vector con todos los elementos
	std::vector<T*> elems;

public:
	InfiniteObjectsManager(int numElems_, Vector3 pos_) : GameObject(pos_), numElems(numElems_) {}
	InfiniteObjectsManager(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numElems_, Vector3 pos_):
		GameObject(gScene_, gPhysics_, pos_), numElems(numElems_) {}

	virtual void update(float t) = 0;
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters() = 0;    // restablece parametros

	inline void setPlayerPos(Vector3 pos_) { playerPos = pos_; } // actualiza la posicion del jugador

	virtual ~InfiniteObjectsManager() {}
};

