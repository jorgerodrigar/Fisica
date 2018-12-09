#include "Water.h"

Water::Water(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, int numGrounds_, Vector3 pos_, float width_, float length_) :
	InfiniteObjectsManager(gScene_, gPhysics_, numGrounds_, pos_), width(width_), length(length_)
{
	// creo numElems 'trozos' de agua, uno detras de otro y los guardo en el vector
	for (int i = 0; i < numElems; i++) {
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(length, 0.1, width));
		RenderItem* renderItem = new RenderItem(shape, { 0, 0, 1, 1 });
		Particle* agua = new Particle(renderItem, { pos.x - length * i, pos.y, pos.z });
		shape->release();
		elems.push_back(agua);

		// cascadas a izquierda y derecha de cada trozo
		Vector3 waterFallPos = { elems[i]->getPosition().x, elems[i]->getPosition().y, elems[i]->getPosition().z - 2 * width / 3 };
		ParticleSystem* waterFall1 = new ParticleSystem(waterFallPos);
		waterFallPos = { elems[i]->getPosition().x, elems[i]->getPosition().y, elems[i]->getPosition().z + 2 * width / 3 };
		ParticleSystem* waterFall2 = new ParticleSystem(waterFallPos);
		waterFalls.push_back({ waterFall1, waterFall2 });
	}

	// direcciones del primer y el ultimo 'trozo'
	first = 0;
	last = numElems - 1;
}

// si el jugador pasa del 'trozo' mas cercano, este se situa detras del ultimo, y asi sucesivamente
// da sensacion de agua infinita usando solo numElems 'trozos'
void Water::update(float t) {
	for (auto e : elems)e->update(t);

	// disparo y actualizo cada cascada
	for (auto w : waterFalls) {
		last_time += t;
		if (last_time > next_time) {
			w.first->Shoot();
			w.second->Shoot();
			next_time = last_time + timeShoot;
		}
		w.first->update(t);
		w.second->update(t);
	}

	if (playerPos.x < elems[first + 1]->getPosition().x) {
		elems[first]->setPosition({ elems[last]->getPosition().x - length, elems[last]->getPosition().y, elems[last]->getPosition().z });
		waterFalls[first].first->setPosition({ elems[last]->getPosition().x, elems[last]->getPosition().y, elems[last]->getPosition().z - 2 * width / 3 });
		waterFalls[first].second->setPosition({ elems[last]->getPosition().x, elems[last]->getPosition().y, elems[last]->getPosition().z + 2 * width / 3 });
		last = first;
		first++;
		if (first >= numElems - 1)first = 0;
	}
}

void Water::resetParameters() {  // los 'trozos' de agua y las cascadas vuelven a su posicion inicial
	first = 0;
	last = numElems - 1;

	for (int i = 0; i < numElems; i++) {
		elems[i]->setPosition({ pos.x - length * i, pos.y, pos.z });
		Vector3 waterFallPos = { elems[i]->getPosition().x, elems[i]->getPosition().y, elems[i]->getPosition().z - 2 * width / 3 };
		waterFalls[i].first->setPosition(waterFallPos);
		waterFallPos = { elems[i]->getPosition().x, elems[i]->getPosition().y, elems[i]->getPosition().z + 2 * width / 3 };
		waterFalls[i].second->setPosition(waterFallPos);
	}
}

void Water::createForcesRegistry() { // le aplico las fuerzas a las cascadas
	for (int i = 0; i < numElems; i++) {
		for (int j = 0; j < forceGenerators.size(); j++) {
			waterFalls[i].first->createForcesRegistry();
			waterFalls[i].second->createForcesRegistry();
			waterFalls[i].first->addForceGenrator(forceGenerators[j]);
			waterFalls[i].second->addForceGenrator(forceGenerators[j]);
		}
	}
}

Water::~Water() {
	for (auto e : elems) {
		delete e;
		e = nullptr;
	}
	for (auto w : waterFalls) {
		delete w.first; w.first = nullptr;
		delete w.second; w.second = nullptr;
	}
}