#pragma once
#include "Particle.h"

enum Tipo {
	AZUL,
	ROJO,
	AMARILLO
};

class FireWork :public Particle
{
private:
	Tipo type;
	Tipo typeOfSon;
	int numOfSons;
	physx::PxShape* shape;
	Vector3 color;
	void initFireWorkRules();

public:
	FireWork(RenderItem* renderItem) :Particle(renderItem) {}

	virtual void update(float t);

	inline void setType(Tipo type_) { 
		type = type_; 
		if (type == AZUL)color = { 073, 103, 141 };
		else if (type == ROJO)color = { 203, 040, 033 };
		else color = { 255, 255, 0 };
	}

	inline void setCount(int count_) {
		numOfSons = count_;
		initFireWorkRules();
	}

	virtual ~FireWork() {}
};

