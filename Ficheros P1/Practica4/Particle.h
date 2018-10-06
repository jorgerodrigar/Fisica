#include "core.hpp"
#include "RenderUtils.hpp"

#pragma once
class Particle: public RenderItem            // clase particula, hereda de RenderItem para poder dibujarse
{
private:
	Vector3 p;                               // posicion                              
	Vector3 v;                               // velocidad 
	Vector3 a;                               // aceleracion
	float damping;                           // rozamiento
	int inverse_mass;                        // masa inversa. Si es 0 la masa sera infinita
	physx::PxTransform transform;            // transform que contiene su posicion y orientacion actuales
	bool active = true;                      // indica si la particula debe ser actualizada y pintada

	void integrate(float t);                 // actualiza los parametros de particula
public:
	Particle(physx::PxShape* _shape, Vector3 p_ = { 0.0, 0.0, 0.0 }, Vector3 v_ = { 0.0, 0.0, 0.0 },
		Vector3 a_ = { 0.0, 0.0, 0.0 }, float damping_ = 0.95, int inverse_mass_ = 1);

	void update(float t) { integrate(t); }

	// --------------------------------------------SETTERS-------------------------------------------------------------
	// posicion
	inline void setPosition(float x, float y, float z) { p = { x, y, z }; }
	inline void setPosition(Vector3 pos) { p = pos; }

	// velocidad
	inline void setVelocity(float x, float y, float z) { v = { x, y, z }; }
	inline void setVelocity(Vector3 vel) { v = vel; }
	inline void setVelocityMagnitude(float vel) { v = getVelocityDirection()*vel; }
	inline void setVelocityDirection(float x, float y, float z);
	inline void setVelocityDirection(Vector3 dir) { v = dir * getVelocityMagnitude(); }

	// aceleracion
	inline void setAcceleration(float x, float y, float z) { a = { x, y, z }; }
	inline void setAcceleration(Vector3 acc) { a = acc; }
	inline void setAccelerationMagnitude(float acc) { a = getAccelerationDirection()*acc; }
	inline void setAccelerationDirection(float x, float y, float z);
	inline void setAccelerationDirection(Vector3 dir) { a = dir * getAccelerationMagnitude(); }

	// otros
	inline void setDamping(float damp) { damping = damp; }
	inline void setMass(int mass) { inverse_mass = 1 / mass; }
	inline void setActive(bool act);

	// ----------------------------------------------GETTERS---------------------------------------------------------------
	// posicion
	inline Vector3 getPosition() { return p; }

	// velocidad
	inline Vector3 getVelocityVector() { return v; }
	inline float getVelocityMagnitude() { return v.magnitude(); }
	inline Vector3 getVelocityDirection() { return v / getVelocityMagnitude(); }

	// aceleracion
	inline Vector3 getAccelerationVector() { return a; }
	inline float getAccelerationMagnitude() { return a.magnitude(); }
	inline Vector3 getAccelerationDirection() { return a / getAccelerationMagnitude(); }

	// otros
	inline int getMass() { return 1 / inverse_mass; }
	inline bool getActive() { return active; }

	~Particle() {}
};

