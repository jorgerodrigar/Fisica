#include "CountDown.h"
#include "GameManager.h"

CountDown::CountDown(GameManager* gm, Vector3 pos):gameManager(gm)
{
	// particulas que se veran afectadas por muelle entre si
	physx::PxShape* shape1 = CreateShape(physx::PxSphereGeometry(3));
	RenderItem* renderItem1 = new RenderItem(shape1, Vector4(0.0, 1.0, 0.0, 1.0));
	p1 = new Particle(renderItem1);
	p1->setDamping(0.6);
	p1->setMaxRecorrido(2000);
	p1->setPosition({pos.x, pos.y, pos.z -50});
	shape1->release();
	physx::PxShape* shape2 = CreateShape(physx::PxSphereGeometry(3));
	RenderItem* renderItem2 = new RenderItem(shape2, Vector4(0.0, 1.0, 0.0, 1.0));
	p2 = new Particle(renderItem2);
	p2->setDamping(0.6);
	p2->setMaxRecorrido(2000);
	p2->setPosition({ pos.x, pos.y, pos.z + 50 });
	shape2->release();

	// fuerzas que afectan a las particulas
	spring1 = new ParticleSpring(p2, 10, 10); // muelle que anclara la primera particula a la segunda
	gameManager->getForces()->push_back(spring1);
	spring2 = new ParticleSpring(p1, 10, 10); // muelle que anclara la segunda particula a la primera
	gameManager->getForces()->push_back(spring2);
	wind1 = new WindForce({ 0, 100, 0 }, 30, pos);
	gameManager->getForces()->push_back(wind1);
	wind2 = new WindForce({ 0, -100, 0 }, 30, { pos.x, pos.y + 60, pos.z });
	gameManager->getForces()->push_back(wind2);

	registry = new ParticleForceRegistry<Particle>();
	registry->add(p1, spring1);
	registry->add(p2, spring2);
	registry->add(p1, wind1);
	registry->add(p2, wind2);
}

void CountDown::update(double t) {
	p1->update(t);
	p2->update(t);
	registry->updateForces(t);

	last_time += t;
	if (last_time > next_time) {
		if (!gameManager->isGameRunning()) {
			gameManager->getPlayer()->startRunning();
			gameManager->setRunning(true);
			setActive(false);
		}
	}
}

void CountDown::resetParameters() {
	setActive(true);
	p1->setToInitialPosition();
	p2->setToInitialPosition();
	p1->clearForce();
	p2->clearForce();
	next_time = last_time + timeOut;
}

void CountDown::setActive(bool active) {
	p1->setActive(active);
	p2->setActive(active);
	spring1->setActive(active);
	spring2->setActive(active);
	wind1->setActive(active);
	wind2->setActive(active);
}

CountDown::~CountDown()
{
	delete p1; p1 = nullptr;
	delete p2; p2 = nullptr;
	delete registry; registry = nullptr;
}
