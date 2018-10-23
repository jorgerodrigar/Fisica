#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "Particle.h"
#include "Pool.h"
#include "FireWorkRules.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

Pool<Particle> pool;
FireWorkRule* rule1;

float last_time = 0;
float next_time = 0;
const float timeShoot = 100; // tiempo que queremos que pase entre particula lanzada y su siguiente (en el sistema de particulas)

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

	pool.setShape(CreateShape(PxSphereGeometry(1)));  //le inidico a la Pool la geometria que quiero que lance
}

// sistema de particulas->salen todas del mismo punto con direccion aleatoria (siempre hacia arriba)
void ParticleSystem(float vel = 75) {
	/*float x, y, z;
	int signoX, signoZ;
	x = rand() % 100 + 1;
	y = rand() % 100 + 1;
	z = rand() % 100 + 1;
	signoX = rand() % 2;
	signoZ = rand() % 2;
	if (signoX == 0)x = -x;
	if (signoZ == 0)z = -z;
	pool.Shoot({ 0, 10, 0 }, { x, y, z });
	pool.setVel(vel);*/

	rule1->setParameters(AZUL, { -5, 25, -5 }, { 5, 28, 5 }, 0.1);
	rule1->create(5);
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	PX_UNUSED(t);

	last_time += t;
	if (last_time > next_time) {
		ParticleSystem();
		next_time = last_time + timeShoot;
	}
	pool.Update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Add custom application code
	// ...

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
	case ' ':
		break;
	case 'F': {
		// disparo una nueva particula con la pos y dir de la camara
		pool.Shoot(GetCamera()->getEye(), GetCamera()->getDir());
		break;
	}
	default:
		break;
	}
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
