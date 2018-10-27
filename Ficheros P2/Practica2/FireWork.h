#pragma once
#include "Particle.h"

enum Tipo {  // tipos de particula que lanazaran los FireWorks
	AZUL,    // el tipo solo determina el color
	ROJO,
	AMARILLO
};

class FireWork :public Particle
{
private:
	Tipo type;     // tipo->color que tendra
	int numOfSons; // numero de hijos que recreara al morir
	Vector3 color;

public:
	FireWork(RenderItem* renderItem) :Particle(renderItem) {}

	bool update(float t);  // ejecuta update de Particle y devuelve si esta activo o no

	//setters
	inline void setType(Tipo type_);
	inline void setCount(int count_) { numOfSons = count_; }

	//getters
	inline Tipo getType() { return type; }

	virtual ~FireWork() {}
};

