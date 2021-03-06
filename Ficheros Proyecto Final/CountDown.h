#pragma once
#include "WindForce.h"
#include "ParticleSpring.h"
#include "ParticleForceRegistry.h"
#include "GameObject.h"

class GameManager;

class CountDown:public GameObject
{
private:
	GameManager* gameManager = nullptr;                     // puntero al gameManager para poder acceder a algunos metodos
	const float timeOut = 30;
	float last_time = 0;
	float next_time = timeOut;
	Particle* p1 = nullptr;                                 // particulas unidas entre si por un muelle
	Particle* p2 = nullptr;
	WindForce* wind1 = nullptr;                             // vientos y muelles que afectan y unen a las particulas
	WindForce* wind2 = nullptr;
	ParticleSpring* spring1 = nullptr;
	ParticleSpring* spring2 = nullptr;
	ParticleForceRegistry<Particle>* registry = nullptr;
public:
	CountDown(GameManager* gm, Vector3 pos_ = { 0, 60, 0 });

	virtual void update(float t);
	virtual void handleEvent(unsigned char key) {}
	virtual void resetParameters();
	void setActive(bool active);

	virtual ~CountDown();
};

