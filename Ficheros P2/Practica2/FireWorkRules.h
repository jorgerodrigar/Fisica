#pragma once
#include "FireWork.h"

struct Payload {
	Tipo type;
	unsigned count;

	void set(Tipo type_, unsigned count_) {
		type = type_;
		count = count_;
	}
};

struct FireWorkRule {
private:
	Tipo type = AZUL;
	Vector3 minVelocity;
	Vector3 maxVelocity;
	float damping = 0.1;
	const Vector3 GRAVITY = { 0.0, -10.0, 0.0 };

public:
	std::vector<Payload>cargas;

	FireWorkRule() {}

	void setParameters(Tipo type_, const Vector3& minVelocity_,
		const Vector3& maxVelocity_, float damping_) {
		type = type_;
		minVelocity = minVelocity_;
		maxVelocity = maxVelocity_;
		damping = damping_;
	}

	void create(int numOfSons_, FireWork* fParent = NULL)const {
		FireWork* fireWork = new FireWork(type, numOfSons_);
		Vector3 vel;
		float velX = 0, velY = 0, velZ = 0;

		if (fParent) {
			Particle* aux = static_cast<Particle*>(fParent);
			fireWork->setPosition(aux->getPosition());
			vel += aux->getVelocityVector();
		}
		else {
			Vector3 start;
			int x = rand() % 3;
			start.x = 5.0*x;
			fireWork->setPosition(start);
		}

		velX += rand() % (int)maxVelocity.x + minVelocity.x;
		velY += rand() % (int)maxVelocity.y + minVelocity.y;
		velZ += rand() % (int)maxVelocity.z + minVelocity.z;

		fireWork->setVelocity({ velX, velY, velZ });
		fireWork->setMass(1);
		fireWork->setDamping(damping);
		fireWork->setAcceleration(GRAVITY);
	}
};