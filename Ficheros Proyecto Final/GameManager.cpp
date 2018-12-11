#include "GameManager.h"

GameManager::GameManager(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_)
{
	// jugador
	player = new Player(gScene_, gPhysics_, PLAYERPOSITION);
	player->setVelocity(PLAYERVELOCITY);
	rigidObjects.push_back(player);

	// gestor de la posicion de la camara (rigidBody situado en la posicion en la que debe estar la camara)
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(5));
	physx::PxTransform transform(CAMERAPOSITION);
	cameraObject = gPhysics_->createRigidDynamic(transform);
	cameraObject->attachShape(*shape);
	gScene_->addActor(*cameraObject);
	shape->release();
	cameraObject->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true); // para que no le afecte la gravedad

	// fuerzas
	gravityBoxes = new GravityForce({ 0, -30, 0 });
	forces.push_back(gravityBoxes);
	gravityWaterFalls = new GravityForce({ 0, -200, 0 });
	forces.push_back(gravityWaterFalls);
	boxesBuoyancy = new ParticleBuoyancy(4, 4, -30, 20);
	forces.push_back(boxesBuoyancy);

	// suelo
	ground = new Ground(gScene_, gPhysics_, 12, GROUNDPOSITION);
	rigidObjects.push_back(ground);

	// obstaculos
	obstacles = new Obstacles(gScene_, gPhysics_, 10, PLAYERPOSITION);
	rigidObjects.push_back(obstacles);

	// agua
	water = new Water(gScene_, gPhysics_, 12, WATERPOSITION);
	water->addForceGenrator(gravityWaterFalls);
	water->createForcesRegistry();
	rigidObjects.push_back(water);

	// cajas flotantes
	boxes = new WaterBoxes(gScene_, gPhysics_, 20, PLAYERPOSITION);
	boxes->addForceGenrator(gravityBoxes);
	boxes->addForceGenrator(boxesBuoyancy);
	boxes->createForcesRegistry();
	rigidObjects.push_back(boxes);

	// managers
	fireWorkManager = new FireWorkManager();
	managers.push_back(fireWorkManager);

	countDown = new CountDown(this); // inicializo el countDown
}

// metodos auxiliares...
void GameManager::infiniteObjectsUpdate() {
	ground->setPlayerPos(player->getObject()->getGlobalPose().p);
	obstacles->setPlayerPos(player->getObject()->getGlobalPose().p);
	water->setPlayerPos(player->getObject()->getGlobalPose().p);
	boxes->setPlayerPos(player->getObject()->getGlobalPose().p);
}
void GameManager::cameraUpdate() {
	if (running)cameraObject->setLinearVelocity({ PLAYERVELOCITY, 0, 0 });    // si estamos jugando, se le aplica al gestor de la camara la misma vel que al jugador
	GetCamera()->setEye(cameraObject->getGlobalPose().p);                     // la camara se pondra en la posicion de su gestor
}
void GameManager::fireWorks() {  // llamado cuando el jugador supera su ultima marca, lanza fuegos artificiales
	fireWorkManager->setPosition({ deadPlayer->getPosition().x - 500, deadPlayer->getPosition().y, deadPlayer->getPosition().z });
	for (int i = 0; i < 4; i++)fireWorkManager->FireworksCreate(AMARILLO);
}
void GameManager::gameLogic(double t) {            // si la camara pasa al jugador, es que ha perdido
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
		countDown->resetParameters();
	}

	if (deadPlayer != nullptr) {      // si el jugador ha muerto y ha superado su ultima marca, lanzamos fuegos artificiales
		deadPlayer->update(t);
		if (!record && player->getObject()->getGlobalPose().p.x < deadPlayer->getPosition().x) {
			fireWorks();
			record = true;
		}
	}
}

void GameManager::update(double t) {
	infiniteObjectsUpdate(); // actualiza la posicion del jugador en todos los objetos 'infinitos'
	
	for (auto o : rigidObjects)o->update(t);
	for (auto m : managers)m->update(t);

	cameraUpdate();         // movimiento de la camara
	gameLogic(t);           // logica del juego
	countDown->update(t);   // actualizo el countDawn
}

void GameManager::handleEvents(unsigned char key) {
	for (auto o : rigidObjects)o->handleEvent(key);
	for (auto m : managers)m->handleEvent(key);
	for (auto f : forces)f->handleEvent(key);

	switch (toupper(key))
	{
	case 'W': {  // al pulsar W la partida comienza
		if (!running) {
			player->startRunning();
			countDown->setActive(false);
			running = true;
		}
		break;
	}
	default:
		break;
	}
}

GameManager::~GameManager()
{
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

	delete countDown; countDown = nullptr;
	delete deadPlayer; deadPlayer = nullptr;
}
