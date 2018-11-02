#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "Particle.h"
#include "Pool.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "WindForce.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

//-------------------------------------------------MIS VARIABLES--------------------------------------------------------

Pool<Particle> pool;                        // pool de particulas
FireWorkManager* fireworkManager = nullptr; // gestor de fuegos artificiales

ParticleForceRegistry* registry = nullptr;  // resgistro donde se guardara cada particula con el generador de fuerzas que le afecte
GravityForce* gravity = nullptr;            // generador de gravedad (todas las particulas lo tendran)
WindForce* windForce = nullptr;             // generador de viento (todas las particulas que esten en su espacio de accion se veran afectadas por el)

float last_time = 0;
float next_time = 0;
const float timeShoot = 0.01; // tiempo que queremos que pase entre particula lanzada y su siguiente (en el sistema de particulas)

//---------------------------------------------------MIS METODOS---------------------------------------------------------

void initVariables();    // inicializo todas las variables y parametros que he creado para no mezclarlos con el resto del codigo
void updateAll(float t); // actualizo todos los sistemas
void ParticleSystem(float vel = 75);

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

void initVariables() {
	pool.setShape(CreateShape(PxSphereGeometry(1))); // le inidico a la Pool la geometria que quiero que lance

	registry = new ParticleForceRegistry();          // registro de particulas con las fuerzas que las afectan
	gravity = new GravityForce({ 0, -5, 0 });       // fuerzas de gravedad y viento (este ultimo solo afectara a las que esten en su radio de accion)
	windForce = new WindForce({ -100, 0, 0 }, 30, { 0, 60, 0 }); // fuerza, radio, posicion

	fireworkManager = new FireWorkManager();         // creo el gestor de fuegos artificiales
	fireworkManager->setForcesRegistry(registry);    // le establezco el registro de fuerzas
	fireworkManager->addForceGenrator(gravity);      // y le digo que a los fireWork les van a afectar estas fuerzas
	fireworkManager->addForceGenrator(windForce);
}

void updateAll(float t) {
	pool.Update(t);
	fireworkManager->FireworksUpdate(t);
	registry->updateForces(t);
}

// sistema de particulas->salen todas del mismo punto con direccion aleatoria (siempre hacia arriba)
void ParticleSystem(float vel) {
	float x, y, z;
	int signoX, signoZ;
	x = rand() % 50 + 1;
	y = rand() % 400 + 1;
	z = rand() % 50 + 1;
	signoX = rand() % 2;
	signoZ = rand() % 2;
	if (signoX == 0)x = -x;
	if (signoZ == 0)z = -z;
	pool.Shoot({ 0, 10, 0 }, { x, y, z });
	pool.setVel(vel);

	// a cada una le afectara la gravedad y el viento
	registry->add(pool.getLastElement(), gravity);  
	registry->add(pool.getLastElement(), windForce);
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
		ParticleSystem();   // genera particulas como una "fuente"
		next_time = last_time + timeShoot;
	}

	updateAll(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Add custom application code
	delete fireworkManager;
	fireworkManager = nullptr;
	delete registry;
	registry = nullptr;
	delete gravity;
	gravity = nullptr;
	delete windForce;
	windForce = nullptr;

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
	case 'F': { // al pulsar F se crea un fuego artificial que va hacia arriba
		fireworkManager->FireworksCreate(AMARILLO);
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
