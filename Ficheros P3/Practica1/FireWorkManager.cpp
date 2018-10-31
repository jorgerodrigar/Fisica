#include "FireWorkManager.h"

FireWorkManager::FireWorkManager() {
	registry = new ParticleForceRegistry();
	//gravity = new GravityForce({ 0, -100, 0 });
	rules.resize(3); // habra tres tipos de reglas
	initFireworkRules();
}

void FireWorkManager::initFireworkRules()
{
	rules[0].setParameters(AMARILLO, 1, 2, Vector3(-250, 250, -250), Vector3(280, 280, 280), 0.1);
	rules[0].cargas.push_back(Payload(AZUL, 10)); //amarillos que disparan azules

	rules[1].setParameters(AZUL, 1, 2, Vector3(-200, 200, -200), Vector3(250, 250, 250), 0.1);
	rules[1].cargas.push_back(Payload(ROJO, 15)); // azules que disparan rojos

	rules[2].setParameters(ROJO, 1, 2, Vector3(-100, 100, -100), Vector3(120, 120, 120), 0.1); // rojos que desaparecen
}

void FireWorkManager::create(Tipo type, unsigned count, FireWork* firework) {
	for (int i = 0; i < count; i++) {
		FireworksCreate(type, firework);
	}
}

FireWorkRule* FireWorkManager::GetRuleFromType(Tipo type) {
	int i = 0;
	while (i < rules.size() && rules[i].getType() != type)i++;
	return &rules[i];
}

FireWork* FireWorkManager::AllocNewFirework() {
	RenderItem* renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), Vector3(1.0, 4.0, 3.0));
	FireWork* firework = new FireWork(renderItem);
	firework->setType(AMARILLO);
	firework->setMaxRecorrido(500);
	fireworks.push_back(firework);
	registry->add(firework, new GravityForce({ 0, -GRAVITY, 0 }));
	return firework;
}

void FireWorkManager::FireworksUpdate(float t) {
	for (int i = 0; i < fireworks.size(); i++)
	{
		if (fireworks[i]->getActive()) {
			if (!fireworks[i]->update(t)) {
				fireworks[i]->setActive(false);
				FireWorkRule* rule = GetRuleFromType(fireworks[i]->getType());
				for (auto itPlayload = rule->cargas.begin(); itPlayload != rule->cargas.end(); itPlayload++)
				{
					Payload payload = (*itPlayload);
					create(payload.type, payload.count, fireworks[i]);
				}
			}
		}
	}

	registry->updateForces(t);
}

void FireWorkManager::FireworksCreate(Tipo type, FireWork* parent)
{
	FireWorkRule* rule = GetRuleFromType(type);
	FireWork* newFirework = AllocNewFirework();
	rule->create(type, newFirework, parent);
}

FireWorkManager::~FireWorkManager() {
	delete registry;
	for (auto it = fireworks.begin(); it < fireworks.end(); it++) {
		delete (*it);
		(*it) = nullptr;
	}
}