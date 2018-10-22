#include "FireWork.h"
#include "FireWorkRules.h"

FireWorkRule rule;

void FireWork::initFireWorkRules() {
	rule.setParameters(type, { -5, 25, -5 }, { 5, 28, 5 }, damping);
	Payload carga;

	for (int i = 0; i < numOfSons; i++) {
		int numOfFollowingSons = rand() % 6;
		carga.set(typeOfSon, numOfFollowingSons);
		rule.cargas.push_back(carga);
	}
}

void FireWork::update(float t) {
	if (!getActive()) {
		for (int i = 0; i < rule.cargas.size(); i++) {
			rule.create(rule.cargas[i].count);
		}
	}
}
