#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"

#include "RenderUtils.hpp"

#include "RigidBodyManager.h"
#include "ParticleBuoyancy.h"
#include "ParticleForceRegistry.h"
#include "ParticleAnchoredSpring.h"
#include "Ground.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;

//---------------------MIS VARIABLES----------------------

// registro de fuerzas que actuan sobre el jugador
ParticleForceRegistry<PxRigidDynamic>* registry = nullptr;

// managers
Ground* ground = nullptr;
PxRigidDynamic* player = nullptr;

std::vector<Manager*> managers;

// fuerzas
ParticleBuoyancy* flotamiento = nullptr;
ParticleAnchoredSpring* muelleCamera = nullptr;
std::vector<ParticleForceGenerator*> forces;

//--------------------------------------------------------

//-----------------------MIS METODOS----------------------

void initMyVariables() {
	// jugador
	PxShape* playerShape = CreateShape(physx::PxSphereGeometry(5));
	physx::PxTransform playerTrans({ 0, 20, 0 });
	player = gPhysics->createRigidDynamic(playerTrans);
	player->attachShape(*playerShape);
	RenderItem* playeRenderItem = new RenderItem(playerShape, player, { 1, 0, 0, 1 });
	physx::PxRigidBodyExt::updateMassAndInertia(*player, 10);
	player->setLinearVelocity({-10, 0, 0});
	gScene->addActor(*player);
	playerShape->release();

	// suelo
	ground = new Ground(gScene, gPhysics, &playerTrans.p, 10);
	managers.push_back(ground);

	// fuerzas
	flotamiento = new ParticleBuoyancy(4, 4, 24, 10);
	//muelleCamera = new ParticleAnchoredSpring(&playerTrans.p, 1, 1);
	forces.push_back(flotamiento);
	forces.push_back(muelleCamera);

	// registro
	registry = new ParticleForceRegistry<PxRigidDynamic>();
	//registry->add(player, muelleCamera);
}

void updateMyVariables(double t) {
	for (auto m : managers)m->update(t);
	registry->updateForces(t);
}

void deleteMyVariables() {
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
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gravity = { 0, -60, 0 }; // añado gravedad
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