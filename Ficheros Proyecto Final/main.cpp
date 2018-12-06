#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "RigidBodyManager.h"
#include "ParticleBuoyancy.h"
#include "ParticleForceRegistry.h"
#include "ParticleAnchoredSpring.h"
#include "Ground.h"
#include "Player.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//---------------------MIS VARIABLES----------------------

// logica de juego
bool playing = false;              // indica si la partida a empezado
const float PLAYERVELOCITY = -100; // velocidad del jugador
const float CAMERAVELOCITY = 0.58; // velocidad de la camara

// objetos
Ground* ground = nullptr;
Player* player = nullptr;

std::vector<RigidObject*> objects;

// managers
std::vector<Manager*> managers;

// fuerzas
ParticleAnchoredSpring* muelleCamera = nullptr;

std::vector<ParticleForceGenerator*> forces;

// registro de fuerzas que actuan sobre el jugador
ParticleForceRegistry<PxRigidDynamic>* registry = nullptr;

//--------------------------------------------------------

//-----------------------MIS METODOS----------------------

void initMyVariables() {
	// jugador
	player = new Player(gScene, gPhysics);
	objects.push_back(player);

	// suelo
	ground = new Ground(gScene, gPhysics, 12);
	objects.push_back(ground);

	// fuerzas
	//muelleCamera = new ParticleAnchoredSpring(&playerTrans.p, 1, 1);
	//forces.push_back(muelleCamera);

	// registro
	registry = new ParticleForceRegistry<PxRigidDynamic>();
	//registry->add(player->getObject(), muelleCamera);
}

void updateMyVariables(double t) {
	ground->setPlayerPos(player->getObject()->getGlobalPose().p);
	for (auto o : objects)o->update(t);
	for (auto m : managers)m->update(t);
	registry->updateForces(t);

	if (playing)GetCamera()->handleAnalogMove(0, CAMERAVELOCITY);
}

void deleteMyVariables() {
	for (auto o : objects) {
		delete o;
		o = nullptr;
	}

	for (auto m : managers) {
		delete m;
		m = nullptr;
	}

	for (auto f : forces) {
		delete f;
		f = nullptr;
	}
}

void keyPressOfMyVariables(unsigned char key) {
	for (auto o : objects)o->handleEvent(key);
	for (auto m : managers)m->handleEvent(key);
	for (auto f : forces)f->handleEvent(key);
}

//-----------------------------------------------------

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.gravity = { 0, -150, 0 }; // a�ado gravedad
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	initMyVariables(); // inicializa todas mis variables y sistemas
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	updateMyVariables(t); // actualiza la logica de todos mis sistemas
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	deleteMyVariables(); // borra todas mis variables y sistemas
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	keyPressOfMyVariables(key); // gestiona los eventos de input de todos mis sistemas

	switch (toupper(key))
	{
	case 'W': {  // al pulsar W la partida comienza
		if (!playing) {
			player->setLinearVelocity({ PLAYERVELOCITY, 0, 0 });
			playing = true;
		}
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
	player->playerCanJump();
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