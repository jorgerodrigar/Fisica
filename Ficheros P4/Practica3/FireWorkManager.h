#pragma once
#include "Manager.h"
#include <vector>
#include "FireWorkRules.h"
#include <queue>
#include <iostream>
#include <vector>

class FireWorkManager:public Manager {                          // clase que almacena y gestiona los fuegos artificiales y sus reglas
private:
	std::vector<FireWorkRule> rules;                            // reglas que seguiran los fireWorks
	std::vector<FireWork*> fireworks;                           // vector de fireWorks
	ParticleForceRegistry* registry = nullptr;                  // resgistro donde se guardara cada fireWork con el generador de fuerzas que le afecte
	std::vector<ParticleForceGenerator*> forceGenerators;       // vector de fuerzas que se aplican a los fireWork

	void initFireworkRules();                                   // inicializa las reglas que tendran los fuegos artificiales
	void create(Tipo type, unsigned count, FireWork* firework); // crea count fuegos de tipo type y padre firework
	FireWorkRule* GetRuleFromType(Tipo type);                   // busca la regla de tipo type
	FireWork* AllocNewFirework();                               // añade un nuevo firework al vector de fireworks
	void FireworksUpdate(float t);                              // actualiza la logica de cada fuego artificial

public:
	FireWorkManager();                                          // inicializa las reglas que tendran los fuegos artificiales

	void FireworksCreate(Tipo type, FireWork* parent = NULL);   // crea un fuego artificial de tipo type y con padre o no

	virtual void update(float t) { FireworksUpdate(t); }
	virtual inline void setForcesRegistry(ParticleForceRegistry* registry_) { registry = registry_; }                 // establece un registro
	virtual inline void addForceGenrator(ParticleForceGenerator* generator) { forceGenerators.push_back(generator); } // añade una fuerza
	virtual void handleEvent(unsigned char key);

	virtual ~FireWorkManager();                                 // elimina todos los fuegos artificiales
};

