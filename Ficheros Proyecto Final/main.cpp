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
#include "Obstacles.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "Water.h"

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

// constantes
const float GRAVITY = -150;                             // gravedad que afectara a los rigidos
const float PLAYERVELOCITY = -100;                      // velocidad del jugador
const Vector3 PLAYERPOSITION = { 0, -30, 0 };           // posicion inicial del jugador
const Vector3 CAMERAPOSITION = { 200.0f, 50.0f, 0.0f }; // posicion inicial de la camara
const Vector3 GROUNDPOSITION = { 0, -35, 0 };           // posicion inicial del suelo
const Vector3 WATERPOSITION = { 0, -36, 0 };            // posicion inicial del agua

// logica de juego
bool running = false;                                   // indica si la partida esta corriendo
bool gameOver = false;                                  // indica si el jugador ha perdido
Particle* deadPlayer = nullptr;                         // representa graficamente la posicion en la que el jugador ha muerto
bool record = false;                                    // flag para controlar que solo se llame una vez a los fuegos cada vez que el jugador supera su marca

// objetos
Ground* ground = nullptr;                               // suelo sobre el que se desarrolla el juego
Player* player = nullptr;                               // jugador
physx::PxRigidDynamic* cameraObject = nullptr;          // objeto que 'movera' la camara
Obstacles* obstacles = nullptr;                         // gestor de obstaculos
Water* water = nullptr;                                 // agua de fondo

std::vector<RigidObject*> rigidObjects;

// managers
FireWorkManager* fireWorkManager = nullptr;

std::vector<Manager*> managers;

// fuerzas
GravityForce* gravityForce = nullptr;

std::vector<ParticleForceGenerator*> forces;

//--------------------------------------------------------

//-----------------------MIS METODOS----------------------

void initMyVariables() {
	// jugador
	player = new Player(gScene, gPhysics, PLAYERPOSITION);
	player->setVelocity(PLAYERVELOCITY);
	rigidObjects.push_back(player);

	// gestor de la posicion de la camara (rigidBody situado en la posicion en la que debe estar la camara)
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(5));
	physx::PxTransform transform(CAMERAPOSITION);
	cameraObject = gPhysics->createRigidDynamic(transform);
	cameraObject->attachShape(*shape);
	gScene->addActor(*cameraObject);
	shape->release();

	// suelo
	ground = new Ground(gScene, gPhysics, 12, GROUNDPOSITION);
	rigidObjects.push_back(ground);

	// obstaculos
	obstacles = new Obstacles(gScene, gPhysics, 10, PLAYERPOSITION);
	rigidObjects.push_back(obstacles);

	// agua
	water = new Water(gScene, gPhysics, 12, WATERPOSITION);
	rigidObjects.push_back(water);

	// fuerzas
	gravityForce = new GravityForce({ 0, -100, 0 });
	forces.push_back(gravityForce);

	// managers
	fireWorkManager = new FireWorkManager();
	managers.push_back(fireWorkManager);
}

// metodos auxiliares de updateMyVariables...
void infiniteObjectsUpdate() {
	ground->setPlayerPos(player->getObject()->getGlobalPose().p);
	obstacles->setPlayerPos(player->getObject()->getGlobalPose().p);
	water->setPlayerPos(player->getObject()->getGlobalPose().p);
}
void cameraUpdate() {
	if (running)cameraObject->setLinearVelocity({ PLAYERVELOCITY, 0, 0 });    // si estamos jugando, se le aplica al gestor de la camara la misma vel que al jugador
	physx::PxTransform transform({ cameraObject->getGlobalPose().p.x, CAMERAPOSITION.y, cameraObject->getGlobalPose().p.z });
	cameraObject->setGlobalPose(transform);                                   // para que no le afecte la gravedad
	GetCamera()->setEye(cameraObject->getGlobalPose().p);                     // la camara se pondra en la posicion de su gestor
}
void fireWorks() {  // llamado cuando el jugador supera su ultima marca, lanza fuegos artificiales
	fireWorkManager->setPosition({ deadPlayer->getPosition().x - 500, deadPlayer->getPosition().y, deadPlayer->getPosition().z });
	for (int i = 0; i < 4; i++)fireWorkManager->FireworksCreate(AMARILLO);
}
void gameLogic(double t) {            // si la camara pasa al jugador, es que ha perdido
	if (cameraObject->getGlobalPose().p.x < player->getObject()->getGlobalPose().p.x + 150) {
		running = false;                                                   
		gameOver = true;

		if (deadPlayer == nullptr) {  // si aun no habia muerto, pintamos la posicion donde lo ha hecho
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(5));
			RenderItem* renderItem = new RenderItem(shape, Vector4(1.0, 0.0, 0.0, 0.0));
			deadPlayer = new Particle(renderItem, player->getObject()->getGlobalPose().p);
			shape->release();
		}                             // si ya habia muerto, actualizamos la posicion donde lo ha hecho
		else deadPlayer->setPosition(player->getObject()->getGlobalPose().p); 
	}

	if (gameOver) {                   // si el jugador ha perdido, reestablecemos parametros
		gameOver = false;
		record = false;
		cameraObject->setLinearVelocity({ 0, 0, 0 });
		cameraObject->setGlobalPose(PxTransform(CAMERAPOSITION));
		for (auto o : rigidObjects)o->resetParameters();
	}

	if (deadPlayer != nullptr) {      // si el jugador ha muerto y ha superado su ultima marca, lanzamos fuegos artificiales
		deadPlayer->update(t);
		if (!record && player->getObject()->getGlobalPose().p.x < deadPlayer->getPosition().x) {
			fireWorks();
			record = true;
		}
	}
}

void updateMyVariables(double t) {
	infiniteObjectsUpdate(); // actualiza la posicion del jugador en todos los objetos 'infinitos'

	for (auto o : rigidObjects)o->update(t);
	for (auto m : managers)m->update(t);

	cameraUpdate();         // movimiento de la camara
	gameLogic(t);           // logica del juego
}

void deleteMyVariables() {
	for (auto o : rigidObjects) {
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

	delete deadPlayer;
	deadPlayer = nullptr;
}

void keyPressOfMyVariables(unsigned char key) {
	for (auto o : rigidObjects)o->handleEvent(key);
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
	sceneDesc.gravity = { 0, GRAVITY, 0 }; // añado gravedad
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
		if (!running) {
			player->startRunning();
			running = true;
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