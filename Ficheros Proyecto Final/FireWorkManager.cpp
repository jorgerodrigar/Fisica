#include "FireWorkManager.h"

FireWorkManager::FireWorkManager() {
	rules.resize(3);       // habra tres tipos de reglas
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
	int i = 0;  // busca un firework desactivado y lo devuelve
	while (i < fireworks.size() && fireworks[i]->getActive())i++;
	if (i < fireworks.size()) {
		fireworks[i]->setActive(true);
		return fireworks[i];
	}
	else {      // si no, crea uno nuevo y lo devuelve
		RenderItem* renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), Vector4(1.0, 4.0, 3.0, 1.0));
		FireWork* firework = new FireWork(renderItem);
		firework->setMaxRecorrido(500);
		fireworks.push_back(firework);
		return firework;
	}
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
}

void FireWorkManager::FireworksCreate(Tipo type, FireWork* parent)
{
	int size = fireworks.size(); // auxiliar, mas tarde comprobaremos el num de fworks ha aumentado (se ha añadido o se ha reutilizado)
	FireWorkRule* rule = GetRuleFromType(type);
	FireWork* newFirework = AllocNewFirework();
	rule->create(type, newFirework, parent);

	// si tenemos un registro y no es un fireWork reutilizado, 
	// le aplicamos al nuevo newFirework todas las fuerzas que tengamos
	if (registry != nullptr && fireworks.size() > size) {
		for (int i = 0; i < forceGenerators.size(); i++)
			registry->add(newFirework, forceGenerators[i]);
	}
}

void FireWorkManager::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case 'F': { // al pulsar F se crea un fuego artificial que va hacia arriba
		FireworksCreate(AMARILLO);
		break;
	}
	default:
		break;
	}
}

FireWorkManager::~FireWorkManager() {
	for (auto it = fireworks.begin(); it < fireworks.end(); it++) {
		delete (*it);
		(*it) = nullptr;
	}
}