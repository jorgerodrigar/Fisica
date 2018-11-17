#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "ParticleSystem.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "WindForce.h"
#include "ExplosionForce.h"
#include "ParticleSpring.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

//-------------------------------------------------MIS VARIABLES--------------------------------------------------------

ParticleForceRegistry* registry = nullptr;  // resgistro donde se guardara cada particula con el generador de fuerzas que le afecte
ParticleSpring* muelle1 = nullptr;          // un muelle para cada particula
ParticleSpring* muelle2 = nullptr;          // para que vayan unidas entre si
Particle* p1 = nullptr;                     // particulas que iran unidas
Particle* p2 = nullptr;

std::vector<ParticleForceGenerator*> generadores; // todos los generadores de fuerza de la escena
std::vector<Particle*> objetos;                   // todos los objetos de la escena (particulas, fireworks, ...)

//---------------------------------------------------MIS METODOS---------------------------------------------------------

void initVariables() {      // inicializa todas mis variables
	registry = new ParticleForceRegistry();                             // registro de particulas con las fuerzas que las afectan

	// particulas que se veran afectadas por muelle entre si
	physx::PxShape* shape1 = CreateShape(physx::PxSphereGeometry(1));
	RenderItem* renderItem1 = new RenderItem(shape1, Vector4(1.0, 4.0, 3.0, 1.0));
	p1 = new Particle(renderItem1);
	p1->setDamping(0.6);
	p1->setMaxRecorrido(2000);
	shape1->release();
	physx::PxShape* shape2 = CreateShape(physx::PxSphereGeometry(1));
	RenderItem* renderItem2 = new RenderItem(shape2, Vector4(1.0, 4.0, 3.0, 1.0));
	p2 = new Particle(renderItem2);
	p2->setDamping(0.6);
	p2->setMaxRecorrido(2000);
	p2->setPosition({ 0, 20, 0 });
	shape2->release();

	objetos.push_back(p1);
	objetos.push_back(p2);

	muelle1 = new ParticleSpring(p2, 10, 10); // muelle que anclara la primera particula a la segunda
	generadores.push_back(muelle1);
	muelle2 = new ParticleSpring(p1, 10, 10); // muelle que anclara la segunda particula a la primera
	generadores.push_back(muelle2);

	registry->add(p1, muelle1);
	registry->add(p2, muelle2);
}

void updateAll(float t) {   // actualiza todos mis sistemas
	registry->updateForces(t);
	for (auto o : objetos)o->update(t);
}

void deleteAll() {   // borra todas mis variables
	delete registry;
	registry = nullptr;
	
	for (auto f : generadores) {
		delete f;
		f = nullptr;
	}

	for (auto o : objetos) {
		delete o;
		o = nullptr;
	}
}

//------------------------------------------------------------------------------------------------------------------------

// Initialize physics engine
// Add custom code at the end of the function
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	initVariables();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	PX_UNUSED(t);

	updateAll(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Add custom application code
	deleteAll();

	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	switch(toupper(key))
	{
	default:
		break;
	}

	for (auto f : generadores)f->handleEvent(key);
}

int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
