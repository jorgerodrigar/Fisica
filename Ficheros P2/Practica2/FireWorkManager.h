#pragma once
#include <vector>
#include "FireWorkRules.h"

class FireWorkManager {
private:
	std::vector<FireWorkRule> rules;
	std::vector<FireWork> fireworks;
public:
	FireWorkManager() { 
		rules.resize(1);
		initFireworkRules(); 
	}

	void initFireworkRules()
	{
		rules[0].setParameters(AMARILLO, Vector3(-5, 25, -5), Vector3(5, 28, 5), 0.1);
		rules[0].getCargas().push_back(Payload(AZUL, 5));
		rules[0].getCargas().push_back(Payload(ROJO, 5));
	}

	void FireworksUpdate(float t) {
		for (auto it = fireworks.begin(); it != fireworks.end(); ++it)
		{
			FireWork firework = (*it);
			if (firework.getActive())
			{
				FireWorkRule* rule = GetRuleFromType(firework.getType());
				for (auto itPlayload = rule->getCargas().begin(); itPlayload != rule->getCargas().end(); itPlayload++)
				{
					Payload payload = (*itPlayload);
					create(payload.type, payload.count, firework);
				}
			}
		}
	}

	void create(Tipo type, unsigned count, FireWork firework) {
		for (int i = 0; i < count; i++) {
			//fireworks.push_back();
		}
	}

	void FireworksCreate(Tipo type, FireWork* parent)
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
		FireWork firework(renderItem);
		fireworks.push_back(firework);
		return &fireworks[fireworks.size() - 1];
	}

	virtual ~FireWorkManager() {}
};

