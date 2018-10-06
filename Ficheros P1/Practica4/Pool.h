#pragma once
#include "core.hpp"
#include <vector>

template <typename T>
class Pool {                                   // clase que crea una Pool de elementos de tipo T
private:
	std::vector<T*> elementos;                 // vector de elementos
	float vel, acc, damping;                   // atributos que tendran los elementos
	physx::PxShape* shape;

	T* getElement() {                          // devuelve el primer elemento de la Pool que este desactivado
		int i = 0;                             // de no encontrarlo devuelve uno nuevo
		while (i < elementos.size() && elementos[i]->getActive())i++;
		if (i < elementos.size())return elementos[i];
		else {
			T* elemento = new T(shape);
			elemento->setActive(false);
			elementos.push_back(elemento);
			return elementos[elementos.size() - 1];
		}
	}

public:
	Pool(float v = 10, float a = 100, float damp = 0.9): 
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

	void setShape(physx::PxShape* shape_) { shape = shape_; }
	void setVel(float v) { vel = v; }
	void setAcc(float a) { acc = a; }
	void setDamp(float damp) { damping = damp; }

	virtual ~Pool() {
		for (int i = 0; i < elementos.size(); i++) {
			delete elementos[i];
			elementos[i] = nullptr;
		}
	}
};

