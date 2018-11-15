#include "ExplosionForce.h"

ExplosionForce::ExplosionForce(float force, float radio_, Vector3 pos, float timeExplosion_) : FORCE(force),
radio(radio_), transform(pos), posIni(pos), timeExplosion(timeExplosion_), last_time(0), next_time(0) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radio)), Vector4(0.0, 1.0, 0.0, 0.0));
	renderItem->addReference();
	renderItem->transform = &transform;
}

void ExplosionForce::updateForce(Particle* particle, float t) {
	if (!particle->hasFiniteMass())return;
	
	if (getActive()) {
		if (explota) {       // si explota, iniciamos el contador
			temporizador(t); // y aplicamos fuerza
			aplicaFuerza(particle);
		}
	}
}

// a las particulas que esten en su zona de accion se les aplicara la fuerza correspondiente
void ExplosionForce::aplicaFuerza(Particle* particle) {
	Vector3 dist = (particle->getPosition() - transform.p);

	if (dist.magnitude() < radio) {
		forceVec = ((dist / dist.magnitude()*FORCE)/dist.magnitude());  // mandara a cada particula lejos del centro de la explosion
		particle->addForce(forceVec*particle->getMass());               // la fuerza sera menor a medida que la particula se aleja del centro
	}
}

// determina cuando puede volver a explotar
void ExplosionForce::temporizador(float t) {
	last_time += t;
	if (next_time == 0)next_time = last_time + timeExplosion;
	if (last_time > next_time) {
		explota = false;
		next_time = last_time + timeExplosion;
	}
}

void ExplosionForce::setActive(bool act) {
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
