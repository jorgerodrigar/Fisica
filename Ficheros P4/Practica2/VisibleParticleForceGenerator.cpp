#include "VisibleParticleForceGenerator.h"

void VisibleParticleForceGenerator::setActive(bool act) {
	ParticleForceGenerator::setActive(act);
	if (!active && registered) {     // si la hemos desactivado y esta registrada no se pinta
		DeregisterRenderItem(renderItem);
		registered = false;
	}
	else if (active && !registered) { // si la hemos activado y no esta registrada volvemos a pintarla
		RegisterRenderItem(renderItem);
		registered = true;
	}
}
