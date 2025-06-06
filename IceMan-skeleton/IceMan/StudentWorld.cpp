#include "StudentWorld.h"
#include <algorithm>
#include <random>
#include <cmath>

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {

}
StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init() {
	player = new IceMan(this, 30, 60);
	iceGridAction(1);
	generateLevelActors();
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move() {
	for (Actor* actor : levelActors) {
		if (actor->isAlive()) {
			actor->move();
		}
	}
	player->move();
	clearIce(player->getX(), player->getY());
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp() {
	delete player;
	player = nullptr;
	iceGridAction(0);
	for (Actor* actor : levelActors) {
		delete actor;
		actor = nullptr;
	}
	levelActors.clear();
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a) {
	levelActors.push_back(a);
}

// Clear a 4x4 region of Ice.
void StudentWorld::clearIce(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			delete iceGrid[x + i][y + j];
			iceGrid[x + i][y + j] = nullptr;
		}
	}
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
	if (0 <= x && x <= 60 && 0 <= y && y <= 60) {
		for (Actor* actor : levelActors) {
			if (!actor->canActorsPassThroughMe()) {
				if (x - 4 < actor->getX() && actor->getX() < x + 4 &&
					y - 4 < actor->getY() && actor->getY() < y + 4) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
	return 0;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {
	
}

// If the IceMan is within radius of a, return a pointer to the
// IceMan, otherwise null.
Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
	return nullptr;
}

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.
Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {
	return nullptr;
}

// Annoy the IceMan.
void StudentWorld::annoyIceMan() {

}

// Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar() {

}

// Give IceMan some water.
void StudentWorld::giveIceManWater() {

}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
	return false;
}

// If the Actor a has a clear line of sight to the IceMan, return
// the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {
	return GraphObject::right;
}

// Return whether the Actor a is within radius of IceMan.
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
	return false;
}

// Determine the direction of the first move a quitting protester
// makes to leave the oil field.
GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y) {
	return GraphObject::right;
}

// Determine the direction of the first move a hardcore protester
// makes to approach the IceMan.
GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y) {
	return GraphObject::right;
}

void StudentWorld::iceGridAction(bool option) {
	int colN = 0;
	for (auto& iceCol : iceGrid) {
		int rowN = 0;
		for (auto& iceBlock : iceCol) {
			if ((colN < 30 || colN > 33 || rowN < 4) && rowN < 60) {
				if (option) {
					iceBlock = new Ice(this, colN, rowN);
				}
				else {
					delete iceBlock;
					iceBlock = nullptr;
				}
			}
			rowN++;
		}
		colN++;
	}
}

void StudentWorld::generateLevelActors() {
	int boulderCount = std::min(currentLevel / 2 + 2, 9);
	int goldCount = std::max(5 - currentLevel / 2, 2);
	int barrelCount = std::min(2 + currentLevel, 21);

	std::srand(static_cast<unsigned int>(time(nullptr)));

	for (int i = 0; i < boulderCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 20, 60, 56);
		addActor(new Boulder(this, newLocation.first, newLocation.second));
		clearIce(newLocation.first, newLocation.second);
	}
	for (int i = 0; i < goldCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60);
		addActor(new GoldNugget(this, newLocation.first, newLocation.second, false));
	}
	for (int i = 0; i < barrelCount; i++) {
		std::pair<int, int> newLocation = findNewLocation(0, 0, 60, 60);
		addActor(new OilBarrel(this, newLocation.first, newLocation.second));
	}
}

std::pair<int, int> StudentWorld::findNewLocation(int x1, int y1, int x2, int y2) {
	for(int loops = 0; loops < 60*60; loops++) {
		int newX = std::rand() % (x2 - x1 + 1) + x1;
		int newY = std::rand() % (y2 - y1 + 1) + y1;
		if (iceGrid[newX][newY] && iceGrid[newX + 3][newY] && iceGrid[newX + 3][newY + 3] && iceGrid[newX][newY + 3]) {
			bool tooClose = false;
			for (Actor* actor : levelActors) {
				if (std::hypot(newX - actor->getX(), newY - actor->getY()) < 6) {
					tooClose = true;
				}
			}
			if (!tooClose) {
				return std::pair<int, int>(newX, newY);
			}
		}
	}
	std::cerr << "findNewLocation() has failed.";
	return std::pair<int, int>(-4, -4);
}





/*

#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {}

int StudentWorld::init() {
	player = new Iceman(keyStorage);
	iceGridAction(1);
	generateLevelActors();
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	if (!getKey(keyStorage)) {
		keyStorage = 0;
	}
	for (auto actor : levelActors) {
		if (actor->isAlive()) {
			actor->doSomething();
		}
	}
	player->doSomething();
	deleteIceAroundActor(player);
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	delete player;
	player = nullptr;
	iceGridAction(0);
	for (Actor* target : levelActors) {
		delete target;
		target = nullptr;
	}
	levelActors.clear();
}

void StudentWorld::iceGridAction(bool option) {
	int colN = 0;
	for (auto& iceCol : iceGrid) {
		int rowN = 0;
		for (auto& iceBlock : iceCol) {
			if ((colN < 30 || colN > 33 || rowN < 4) && rowN < 60) {
				if (option) {
					iceBlock = new Ice(colN, rowN);
				}
				else {
					delete iceBlock;
					iceBlock = nullptr;
				}
			}
			rowN++;
		}
		colN++;
	}
}

void StudentWorld::deleteIceAroundActor(Actor* target) {
	int px = target->getX();
	int py = target->getY();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			delete iceGrid[px + i][py + j];
			iceGrid[px + i][py + j] = nullptr;
		}
	}
}

void StudentWorld::generateLevelActors() {
	int B = std::min(currentLevel / 2 + 2, 9);
	int G = std::max(5 - currentLevel / 2, 2);
	int L = std::min(2 + currentLevel, 21);

	std::vector<std::pair<int, int>> takenLocations;

	std::srand(std::time(nullptr));

	placeActors<Ice>(B, 0, 20, 60, 56, takenLocations); // Boulder <------------------------------------------------------------------------------------
	placeActors<Ice>(G, 0, 0, 60, 60, takenLocations); // Gold <------------------------------------------------------------------------------------
	placeActors<Ice>(L, 0, 0, 60, 60, takenLocations); // Oil <------------------------------------------------------------------------------------
}

std::pair<int, int> StudentWorld::generateLocation(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& takenLocations) {
	bool goodCoords = false;
	int newX;
	int newY;
	while (!goodCoords) {
		goodCoords = true;
		newX = std::rand() % (x2 - x1 + 1) + x1;
		newY = std::rand() % (y2 - y1 + 1) + y1;
		if (takenLocations.empty() && iceGrid[newX][newY] && iceGrid[newX + 4][newY + 4]) { goodCoords = true; }
		for (std::pair<int, int> checkCoord : takenLocations) {
			float distance = std::hypot(newX - checkCoord.first, newY - checkCoord.second);
			if (distance < 6 || !iceGrid[newX][newY] || !iceGrid[newX + 3][newY] || !iceGrid[newX][newY + 3] || !iceGrid[newX+3][newY+3]) { goodCoords = false; }
		}
	}
	return std::pair<int, int>(newX, newY);
}

*/