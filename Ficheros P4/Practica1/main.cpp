#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "ParticleSystem.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "WindForce.h"
#include "ExplosionForce.h"
#include "ParticleAnchoredSpring.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

//-------------------------------------------------MIS VARIABLES--------------------------------------------------------

//ParticleSystem particleSystem;              // gestor del sistema de particulas
//FireWorkManager* fireworkManager = nullptr; // gestor de fuegos artificiales

ParticleForceRegistry* registry = nullptr;  // resgistro donde se guardara cada particula con el generador de fuerzas que le afecte
GravityForce* gravity = nullptr;            // generador de gravedad (todas las particulas lo tendran)
WindForce* windForce = nullptr;             // generador de viento (todas las particulas que esten en su espacio de accion se veran afectadas por el)
ExplosionForce* explosion = nullptr;        // generador de explosiones (al pulsar la tecla 'e' todas las particulas en su radio se veran afectadas)
ParticleAnchoredSpring* muelle = nullptr;
Particle* particle = nullptr;

float last_time = 0;
float next_time = 0;
const float timeShoot = 0.01; // tiempo que queremos que pase entre particula lanzada y su siguiente (en el sistema de particulas)

//---------------------------------------------------MIS METODOS---------------------------------------------------------

void initVariables() {      // inicializa todas mis variables
	registry = new ParticleForceRegistry();          // registro de particulas con las fuerzas que las afectan
	gravity = new GravityForce({ 0, -15, 0 });        // fuerzas de gravedad y viento (este ultimo solo afectara a las que esten en su radio de accion)
	windForce = new WindForce({ -100, 0, 0 }, 30, { 0, 40, 0 });       // vector fuerza, radio, posicion
	//explosion = new ExplosionForce(20000, 30, { 0, 40, 0 }, 30);       // modulo fuerza, radio, posicion y tiempo hasta proxima explosion (en milisegundos)
	muelle = new ParticleAnchoredSpring(new Vector3(0, 10, 0), 10, 10);

	/*fireworkManager = new FireWorkManager();         // creo el gestor de fuegos artificiales
	fireworkManager->setForcesRegistry(registry);    // le establezco el registro de fuerzas
	fireworkManager->addForceGenrator(gravity);      // y le digo que a los fireWork les van a afectar estas fuerzas
	//fireworkManager->addForceGenrator(windForce);
	fireworkManager->addForceGenrator(explosion);

	particleSystem.setForcesRegistry(registry);      // lo mismo con el sistema de partículas
	particleSystem.addForceGenrator(gravity);
	//particleSystem.addForceGenrator(windForce);
	particleSystem.addForceGenrator(explosion);*/

	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1));
	RenderItem* renderItem = new RenderItem(shape, Vector4(1.0, 4.0, 3.0, 1.0));
	particle = new Particle(renderItem);
	particle->setDamping(0.9);
	shape->release();

	registry->add(particle, muelle);
	registry->add(particle, windForce);
	registry->add(particle, gravity);
}

void updateAll(float t) {   // actualiza todos mis sistemas
	//particleSystem.update(t);
	//fireworkManager->FireworksUpdate(t);
	registry->updateForces(t);
	particle->update(t);
}

void deleteAll() {   // borra todas mis variables
	//delete fireworkManager;
	//fireworkManager = nullptr;
	delete registry;
	registry = nullptr;
	delete gravity;
	gravity = nullptr;
	delete windForce;
	windForce = nullptr;
	delete explosion;
	explosion = nullptr;
	delete muelle;
	muelle = nullptr;
	delete particle;
	particle = nullptr;
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

	last_time += t;
	if (last_time > next_time) {
		//particleSystem.shoot();   // genera particulas como una "fuente"
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
	case ' ': { // al pulsar espacio se desactiva el tiempo
		bool activado = windForce->getActive();
		windForce->setActive(!activado);
		break;
	}
	/*case 'E': { // al pulsar E la fuerza de explosion explota en su posicion inicial
		explosion->setInitialPosition();
		explosion->Explota();
		break;
	}
	case 'R': { // al pulsar R la fuerza de explosion explota en la posicion en la que estemos mirando
		explosion->setPosition(GetCamera()->getEye() + GetCamera()->getDir() * (GetCamera()->getEye().z + explosion->getRadio()));
		explosion->Explota();
		break;
	}*/
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
