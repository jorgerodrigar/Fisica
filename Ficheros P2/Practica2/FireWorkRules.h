#pragma once
#include "FireWork.h"
#include <vector>

// representa las cargas
struct Payload {
	Tipo type;      // tipo de la carga (color)
	unsigned count; // numero de cargas que generara

	Payload(Tipo type_, unsigned count_) :type(type_), count(count_) {}

	void set(Tipo type_, unsigned count_) {
		type = type_;
		count = count_;
	}
};

// representa las reglas que seguiran los fireworks para crearse
struct FireWorkRule {
private:                // parametros del FireWork
	Tipo type = AZUL;
	Vector3 minVelocity;
	Vector3 maxVelocity;
	float damping = 0.1;
	const Vector3 GRAVITY = { 0.0, -10.0, 0.0 };
	std::vector<Payload>cargas;

public:
	FireWorkRule() {}

	void setParameters(Tipo type_, const Vector3& minVelocity_,
		const Vector3& maxVelocity_, float damping_) {
		type = type_;
		minVelocity = minVelocity_;
		maxVelocity = maxVelocity_;
		damping = damping_;
	}

	// crea un nuevo FireWork 
	void create(Tipo type, FireWork* firework, FireWork* fParent = NULL)const {
		firework->setType(type);

		Vector3 vel;
		float velX = 0, velY = 0, velZ = 0;

		if (fParent) {
			Particle* aux = static_cast<Particle*>(fParent);
			firework->setPosition(aux->getPosition());
			vel += aux->getVelocityVector();
		}
		else {
			Vector3 start;
			int x = rand() % 3;
			start.x = 5.0*x;
			firework->setPosition(start);
		}

		velX += rand() % (int)maxVelocity.x + minVelocity.x;
		velY += rand() % (int)maxVelocity.y + minVelocity.y;
		velZ += rand() % (int)maxVelocity.z + minVelocity.z;

		firework->setVelocity({ velX, velY, velZ });
		firework->setMass(1);
		firework->setDamping(damping);
		firework->setAcceleration(GRAVITY);
	}

	inline Tipo getType() { return type; }
	inline std::vector<Payload> getCargas() { return cargas; }
};