#pragma once

#include "RigidBodyManager.h"
#include "ParticleBuoyancy.h"
#include "ParticleAnchoredSpring.h"
#include "Ground.h"
#include "Player.h"
#include "Obstacles.h"
#include "FireWorkManager.h"
#include "GravityForce.h"
#include "Water.h"
#include "WaterBoxes.h"
#include "CountDown.h"

using namespace physx;

class GameManager  //---------------------------GESTOR DEL JUEGO--------------------------------
{
private:
	// constantes
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
	CountDown* countDown = nullptr;                         // cuenta atras para iniciar el juego (salvo que se pulse W)

	// objetos
	Ground* ground = nullptr;                               // suelo sobre el que se desarrolla el juego
	Player* player = nullptr;                               // jugador
	physx::PxRigidDynamic* cameraObject = nullptr;          // objeto que 'movera' la camara
	Obstacles* obstacles = nullptr;                         // gestor de obstaculos
	Water* water = nullptr;                                 // agua de fondo
	WaterBoxes* boxes = nullptr;                            // cajas que flotan sobre el agua

	// managers
	FireWorkManager* fireWorkManager = nullptr;             // fuegos artificiales que se lanzaran cuando el jugador supere su anterior marca

	// fuerzas
	GravityForce* gravityBoxes = nullptr;                   // gravedad que afecta a las cajas flotantes
	GravityForce* gravityWaterFalls = nullptr;              // gravedad que afecta a las fuentes de agua
	ParticleBuoyancy* boxesBuoyancy = nullptr;              // flotamiento de las cajas

	// vectores donde englobo los distintos sistemas
	std::vector<GameObject*> gameObjects;
	std::vector<Manager*> managers;
	std::vector<ParticleForceGenerator*> forces;

	//-----------------METODOS-------------------
	void infiniteObjectsUpdate();
	void cameraUpdate();
	void fireWorks();
	void gameLogic(double t);

public:
	GameManager(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_);

	void update(double t);
	void handleEvents(unsigned char key);
	inline Player* getPlayer() { return player; }
	inline std::vector<ParticleForceGenerator*>* getForces() { return &forces; }
	inline bool isGameRunning() { return running; }
	inline void setRunning(bool run) { running = run; }

	virtual ~GameManager();
};

