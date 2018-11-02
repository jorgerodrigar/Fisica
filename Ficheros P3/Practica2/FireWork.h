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
	Tipo type;     // tipo-> color que tendra
	float age;     // edad-> al llegar a 0 muere
public:
	FireWork(RenderItem* renderItem) :Particle(renderItem) {}

	bool update(float t) {
		Particle::update(t); // ejecuta update de Particle y devuelve si debe morir o no
		age -= t;            // actualizo edad
		return age >= 0;
	}

	//setters
	inline void setType(Tipo type_) {
		type = type_;  // establece el color del FireWork dependiendo del tipo recibido
		if (type == AZUL)renderItem->color = { 0, 1, 1, 0 };
		else if (type == ROJO)renderItem->color = { 1, 0, 0, 0 };
		else renderItem->color = { 0, 1, 0, 0 };
	}
	inline void setAge(float age_) { age = age_; }

	//getters
	inline Tipo getType() { return type; }

	virtual ~FireWork() {}
};

