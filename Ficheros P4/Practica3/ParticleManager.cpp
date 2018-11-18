#include "ParticleManager.h"

void ParticleManager::handleEvent(unsigned char key) {
	switch (toupper(key))
	{
	case 'T': {
		// dispara una nueva particula con la pos y dir de la camara
		shoot(GetCamera()->getEye(), GetCamera()->getDir());
		break;
	}
	default:
		break;
	}
}
