#include "FireWork.h"
#include "FireWorkRules.h"

FireWorkRule rule;

bool FireWork::update(float t) {
	Particle::update(t);
	return getActive();
}

// establece el tipo y el color correspondiente
inline void FireWork::setType(Tipo type_) { 
	type = type_;
	if (type == AZUL)color = { 073, 103, 141 };
	else if (type == ROJO)color = { 203, 040, 033 };
	else color = { 255, 255, 0 };
}
