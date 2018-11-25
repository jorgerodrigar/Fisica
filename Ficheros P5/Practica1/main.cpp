#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"

#include "RenderUtils.hpp"

#include "RigidBodyManager.h"
#include "ExplosionForce.h"

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

// managers
PxRigidStatic* suelo = nullptr;
RigidBodyManager* rigidBodyManager = nullptr;

std::vector<Manager*> managers;

// fuerzas
ExplosionForce* explosion = nullptr;
std::vector<ParticleForceGenerator*> forces;

// temporizador para que RigidBodyManager dispare
float last_time = 0;
float next_time = 0;
const float timeShoot = 0.5;

//--------------------------------------------------------

//-----------------------MIS METODOS----------------------

void initMyVariables() {
	// suelo
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(100, 0.1, 100));
	PxTransform transform({ 0, 0, 0 });
	suelo = gPhysics->createRigidStatic(transform);
	suelo->attachShape(*shape);
	RenderItem* renderItem = new RenderItem(shape, suelo, { 0, 1, 0, 1 });
	gScene->addActor(*suelo);
	shape->release();

	// generador de rigid bodys
	rigidBodyManager = new RigidBodyManager(gScene, gPhysics, { 0, 20, 0 });
	managers.push_back(rigidBodyManager);

	// fuerzas
	explosion = new ExplosionForce(20000, 30, { 0, 40, 0 }, 1);
	forces.push_back(explosion);

	rigidBodyManager->createForcesRegistry();
	rigidBodyManager->addForceGenrator(explosion);
}

void updateMyVariables(double t) {
	last_time += t;
	if (last_time > next_time) {
		rigidBodyManager->Shoot();
		next_time = last_time + timeShoot;
	}
	for (auto m : managers)m->update(t);
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