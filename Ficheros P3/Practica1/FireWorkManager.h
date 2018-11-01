#pragma once
#include <vector>
#include "FireWorkRules.h"
#include <queue>
#include <iostream>
#include "ParticleForceRegistry.h"
#include "GravityForce.h"

class FireWorkManager {                        // clase que almacena y gestiona los fuegos artificiales y sus reglas
private:
	std::vector<FireWorkRule> rules;           // reglas que seguiran los fireWorks
	std::vector<FireWork*> fireworks;          // vector de fireWorks
	ParticleForceRegistry* registry = nullptr; // resgistro donde se guardara cada fireWork con el generador de fuerzas que le afecte
	GravityForce* gravity;                     // generador de gravedad (todos los fireWork lo tendran)

	void initFireworkRules();                                   // inicializa las reglas que tendran los fuegos artificiales
	void create(Tipo type, unsigned count, FireWork* firework); // crea count fuegos de tipo type y padre firework
	FireWorkRule* GetRuleFromType(Tipo type);                   // busca la regla de tipo type
	FireWork* AllocNewFirework();                               // añade un nuevo firework al vector de fireworks

public:
	FireWorkManager();                                          // inicializa las reglas que tendran los fuegos artificiales

	void FireworksUpdate(float t);                              // actualiza la logica de cada fuego artificial
	void FireworksCreate(Tipo type, FireWork* parent = NULL);   // crea un fuego artificial de tipo type y con padre o no

	virtual ~FireWorkManager();                                 // elimina todos los fuegos artificiales
};

