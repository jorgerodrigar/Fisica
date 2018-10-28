#pragma once
#include <vector>
#include "FireWorkRules.h"
#include <queue>

class FireWorkManager {
private:
	std::vector<FireWorkRule> rules;   // reglas que seguiran los fireWorks
	std::vector<FireWork*> fireworks;  // vector de fireWorks
	std::queue<FireWork*> nuevosElementos;
public:
	FireWorkManager() { 
		rules.resize(3);
		initFireworkRules(); 
	}

	void initFireworkRules()
	{
		rules[0].setParameters(AMARILLO, 1, 2, Vector3(-250, 250, -250), Vector3(280, 280, 280), 0.1);
		for (int i = 0; i < 6; i++)rules[0].cargas.push_back(Payload(AZUL, 5));

		rules[1].setParameters(AZUL, 1, 2, Vector3(-200, 200, -200), Vector3(250, 250, 250), 0.1);
		for (int i = 0; i < 4; i++)rules[1].cargas.push_back(Payload(ROJO, 5));

		rules[2].setParameters(ROJO, 1, 2, Vector3(-100, 100, -100), Vector3(120, 120, 120), 0.1);
	}

	void FireworksUpdate(float t) {
		while (!nuevosElementos.empty()) {
			fireworks.push_back(nuevosElementos.front());
			nuevosElementos.pop();
		}

		for (auto it = fireworks.begin(); it < fireworks.end(); it++)
		{
			FireWork* firework = (*it);
			if (firework->getActive()) {
				if (!firework->update(t)) {
					firework->setActive(false);
					FireWorkRule* rule = GetRuleFromType(firework->getType());
					for (auto itPlayload = rule->cargas.begin(); itPlayload != rule->cargas.end(); itPlayload++)
					{
						Payload payload = (*itPlayload);
						create(payload.type, payload.count, firework);
					}
				}
			}
			//else fireworks.erase(it);
		}
	}

	void create(Tipo type, unsigned count, FireWork* firework) {
		for (int i = 0; i < count; i++) {
			FireworksCreate(type, firework);
		}
	}

	void FireworksCreate(Tipo type, FireWork* parent = NULL)
	{
		FireWorkRule* rule = GetRuleFromType(type);
		FireWork* newFirework = AllocNewFirework();
		rule->create(type, newFirework, parent);
	}

	FireWorkRule* GetRuleFromType(Tipo type) {
		int i = 0;
		while (i < rules.size() && rules[i].getType() != type)i++;
		return &rules[i];
	}

	FireWork* AllocNewFirework() {
		RenderItem* renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), Vector3(1.0, 4.0, 3.0));
		FireWork* firework = new FireWork(renderItem);
		firework->setType(AMARILLO);
		firework->setMaxRecorrido(500);
		EncolaElemento(firework);
		return firework;
	}

	inline void EncolaElemento(FireWork* fireWork) {
		nuevosElementos.push(fireWork);
	}

	virtual ~FireWorkManager() { 
		for (auto it = fireworks.begin(); it < fireworks.end(); it++) {
			delete (*it);
			(*it) = nullptr;
		}
	}
};

