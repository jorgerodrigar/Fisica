#pragma once
#include "Particle.h"

struct FireWorkRule;

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
	FireWork(Tipo type_, int numOfSons_) :type(type_) {
		if (type == AZUL)color = { 073, 103, 141 };
		else if (type == ROJO)color = { 203, 040, 033 };
		else color = { 255, 255, 0 };
		shape = CreateShape(physx::PxSphereGeometry(1));

		RenderItem* renderItem = new RenderItem(shape, color);

		numOfSons = numOfSons_;

		initFireWorkRules();

		Particle::Particle(renderItem);
	}

	virtual void update(float t);

	virtual ~FireWork() {}
};

