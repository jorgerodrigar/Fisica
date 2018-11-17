#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "ParticleSystem.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "WindForce.h"
#include "ExplosionForce.h"
#include "ParticleBuoyancy.h"
#include "RenderObject.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

//-------------------------------------------------MIS VARIABLES--------------------------------------------------------

ParticleForceRegistry* registry = nullptr;  // resgistro donde se guardara cada particula con el generador de fuerzas que le afecte
GravityForce* gravity = nullptr;            // generador de gravedad (todas las particulas lo tendran)
ParticleBuoyancy* flotamiento = nullptr;    // generador de flotamiento
Particle* p1 = nullptr;                     // particula que flotara
RenderObject water;                         // representacion grafica de agua

std::vector<ParticleForceGenerator*> generadores; // vector de todos los generadores de la escena
std::vector<Particle*> objetos;                   // vector de todos los objetos de la escena (particulas, fireworks, ...)

float last_time = 0;
float next_time = 0;
const float timeShoot = 0.01; // tiempo que queremos que pase entre particula lanzada y su siguiente (en el sistema de particulas)

//---------------------------------------------------MIS METODOS---------------------------------------------------------

void initVariables() {      // inicializa todas mis variables
	registry = new ParticleForceRegistry();                             // registro de particulas con las fuerzas que las afectan
	gravity = new GravityForce({ 0, -15, 0 });                          // fuerza de gravedad
	generadores.push_back(gravity);
	flotamiento = new ParticleBuoyancy(4, 4, 24, 10);                   // fuerza de flotamiento
	generadores.push_back(flotamiento);

	// agua
	water = RenderObject(physx::PxBoxGeometry(50, 0.1, 50), { 0, 20, 0 }, Vector4(0.0, 0.0, 1.0, 1.0));

	// particula que se vera afectada por el flotamiento
	physx::PxShape* shape1 = CreateShape(physx::PxSphereGeometry(1));
	RenderItem* renderItem1 = new RenderItem(shape1, Vector4(1.0, 4.0, 3.0, 1.0));
	p1 = new Particle(renderItem1);
	p1->setDamping(0.3);
	shape1->release();

	objetos.push_back(p1);

	// a la particula le afectara el flotamiento y la gravedad
	registry->add(p1, flotamiento);
	registry->add(p1, gravity);
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
	case 'B':
		break;
	case '+': {
		p1->increaseMass();
		break;
	}
	case '-': {
		p1->decreaseMass();
		break;
	}
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
