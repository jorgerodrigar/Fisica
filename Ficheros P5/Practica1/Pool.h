#pragma once
#include "core.hpp"
#include <vector>
#include "ParticleForceRegistry.h"
#include "ParticleForceGenerator.h"

template <typename T>
class Pool {                                   // clase que crea una Pool de elementos de tipo T
private:
	std::vector<T*> elementos;                 // vector de elementos
	float vel, acc, damping;                   // atributos que tendran los elementos
	physx::PxShape* shape = nullptr;
	ParticleForceRegistry<T>* registry = nullptr;           // resgistro donde se guardara cada elemento con el generador de fuerzas que le afecte
	std::vector<ParticleForceGenerator*> forceGenerators;   // vector de fuerzas que se aplican a los elementos

	T* getElement() {                          // devuelve el primer elemento de la Pool que este desactivado
		int i = 0;                             // de no encontrarlo devuelve uno nuevo
		while (i < elementos.size() && elementos[i]->getActive())i++;
		if (i < elementos.size())return elementos[i];
		else {
			elementos.push_back(CreateElement());
			return elementos[elementos.size() - 1];
		}
	}

	T* CreateElement() {
		if (shape == nullptr)shape = CreateShape(physx::PxSphereGeometry(1));        // en caso de no haber establecido ningun shape creamos una esfera por defecto
		RenderItem* renderItem = new RenderItem(shape, Vector4(1.0, 4.0, 3.0, 1.0)); // creamos el item que representara graficamente al elemento
		T* elemento = new T(renderItem);                                             // creamos el elemento
		elemento->setActive(false);                                                  // lo desactivamos
		shape->release();                                                            // liberamos el shape

		if (registry != nullptr) {                                                   // si tenemos un registro, añadimos a el el nuevo elemento creado con todos
			for (int i = 0; i < forceGenerators.size(); i++)                         // los generadores que le afecten
				registry->add(elemento, forceGenerators[i]);
		}

		return elemento;
	}

public:
	Pool(float v = 10, float a = 100, float damp = 0.9) :
		vel(v), acc(a), damping(damp) {}

	// actualiza todos los elementos
	void Update(float t) { for (int i = 0; i < elementos.size(); i++)elementos[i]->update(t); }

	// dispara el primer elemento no activo de la Pool o uno nuevo en su defecto
	void Shoot(Vector3 pos, Vector3 dir) {
		T* elemento = getElement();
		elemento->setPosition(pos);           // lo pongo en la posicion deseada
		elemento->setVelocity(dir*vel);       // le aplico velocidad en la direccion deseada
		elemento->setAcceleration(dir*acc);   // le aplico aceleracion en la direccion deseada
		elemento->setDamping(damping);        // le añado un poco de rozamiento
		elemento->setActive(true);            // lo activo para que se actualice y pinte
	}

	T* getLastElement() { return elementos[elementos.size() - 1]; }  // devuelve el ultimo elemento añadido a la pool

	void setShape(physx::PxShape* shape_) { shape = shape_; }
	void setVel(float v) { vel = v; }
	void setAcc(float a) { acc = a; }
	void setDamp(float damp) { damping = damp; }
	inline void setForcesRegistry(ParticleForceRegistry<T>* registry_) { registry = registry_; }              // establece un registro
	inline void addForceGenrator(ParticleForceGenerator* generator) { forceGenerators.push_back(generator); } // añade una fuerza

	virtual ~Pool() {
		for (int i = 0; i < elementos.size(); i++) {
			delete elementos[i];
			elementos[i] = nullptr;
		}
	}
};

