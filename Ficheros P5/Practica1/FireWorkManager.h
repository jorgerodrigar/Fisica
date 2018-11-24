#pragma once
#include "PManager.h"
#include <vector>
#include "FireWorkRules.h"
#include <queue>
#include <iostream>
#include <vector>

class FireWorkManager:public PManager {                          // clase que almacena y gestiona los fuegos artificiales y sus reglas
private:
	std::vector<FireWorkRule> rules;                            // reglas que seguiran los fireWorks
	std::vector<FireWork*> fireworks;                           // vector de fireWorks

	void initFireworkRules();                                   // inicializa las reglas que tendran los fuegos artificiales
	void create(Tipo type, unsigned count, FireWork* firework); // crea count fuegos de tipo type y padre firework
	FireWorkRule* GetRuleFromType(Tipo type);                   // busca la regla de tipo type
	FireWork* AllocNewFirework();                               // añade un nuevo firework al vector de fireworks
	void FireworksUpdate(float t);                              // actualiza la logica de cada fuego artificial

public:
	FireWorkManager();                                          // inicializa las reglas que tendran los fuegos artificiales

	void FireworksCreate(Tipo type, FireWork* parent = NULL);   // crea un fuego artificial de tipo type y con padre o no

	virtual void update(float t) { FireworksUpdate(t); }
	virtual void handleEvent(unsigned char key);

	virtual ~FireWorkManager();                                 // elimina todos los fuegos artificiales
};

