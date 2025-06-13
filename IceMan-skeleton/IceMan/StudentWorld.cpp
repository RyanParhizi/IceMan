#include "StudentWorld.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <sstream>
#include <iomanip>

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
	targetProtestorCount = std::min(15, int(2 + currentLevel * 1.5));
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move() {
	player->move();

	std::for_each(levelActors.begin(), levelActors.end(), // Call All Actors 
		[](Actor* actor) { actor->move(); });

	levelActors.erase(std::remove_if(levelActors.begin(), levelActors.end(), // Remove Dead Actors
		[](Actor* actor) { if (!actor->isAlive()) { delete actor; return true; } return false;}), levelActors.end());

	clearIce(player->getX(), player->getY());

	addNewActors();

	updateDisplayText();

	if (oilLeft <= 0) { // Win Condition
		currentLevel++;
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (player->getHitPoints() <= 0) { // Lose Condition
		livesLeft--;
		playSound(SOUND_PLAYER_GIVE_UP);
		if (livesLeft <= 0) { // Game Over Condition
			//return GWSTATUS_GAME_OVER???
		}
		return GWSTATUS_PLAYER_DIED;
	}

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
	bool icePresent = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (iceGrid[x + i][y + j]){
				icePresent = true;
			}
			delete iceGrid[x + i][y + j];
			iceGrid[x + i][y + j] = nullptr;
		}
	}
	if (icePresent) {
		playSound(SOUND_DIG);
	}
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
	if (x < 0 || 60 < x || y < 0 || 60 < y) {
		return false;
	}
	for (Actor* actor : levelActors) {
		if (!actor->canActorsPassThroughMe() && actor != a) {
			if (x - 4 < actor->getX() && actor->getX() < x + 4 &&
				y - 4 < actor->getY() && actor->getY() < y + 4) {
				return false;
			}
		}
	}
	if (a != player) {
		if (!allIceAtCoords(x, y, false)) {
			return false;
		}
	}
	return true;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
	int annoiedActorCount = 0;
	for (Actor* actor : levelActors) {
		if (actor == annoyer) {
			continue;
		}
		if (std::hypot(annoyer->getX() - actor->getX(), annoyer->getY() - actor->getY()) <= radius) {
			actor->annoy(points);
			annoiedActorCount++;
		}
	}
	if(annoyer->canHurtIceMan()) {
		if (std::hypot(annoyer->getX() - player->getX(), annoyer->getY() - player->getY()) <= radius) {
			player->annoy(points);
			annoiedActorCount++;
		}
	}
	return annoiedActorCount;
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

bool StudentWorld::allIceAtCoords(int x, int y, bool mod) const {
	bool allIce = true;
	bool noIce = true;

	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (iceGrid[i][j]) {
				noIce = false;
			}
			if (!iceGrid[i][j]) {
				allIce = false;
			}
		}
	}

	if (mod == true) { return allIce; }
	if (mod == false) { return noIce; }

}

std::pair<int, int> StudentWorld::findNewLocation(int x1, int y1, int x2, int y2) { // Under the ice for starting actors
	for(int loops = 0; loops < 60*60; loops++) {
		int newX = std::rand() % (x2 - x1 + 1) + x1;
		int newY = std::rand() % (y2 - y1 + 1) + y1;
		if (allIceAtCoords(newX, newY, true)) {
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
	return std::pair<int, int>(0, 0);
}

std::pair<int, int> StudentWorld::findNewLocation2(int x1, int y1, int x2, int y2) { // In a clearing for water pools // Consider combining with previous version somehow.
	for (int loops = 0; loops < 60 * 60; loops++) {
		int newX = std::rand() % (x2 - x1 + 1) + x1;
		int newY = std::rand() % (y2 - y1 + 1) + y1;
		if (allIceAtCoords(newX, newY, false)) {
				return std::pair<int, int>(newX, newY);
		}
	}
	std::cerr << "findNewLocation() has failed.";
	return std::pair<int, int>(0, 0);
}


bool StudentWorld::getTrueByChance(double chance) {
	int roll = rand();
	double normalized = static_cast<double>(roll) / RAND_MAX;
	return normalized < chance;
}

void StudentWorld::createHardcoreProtestor() {
	addActor(new HardcoreProtester(this, 60, 60, IID_HARD_CORE_PROTESTER));
}

void StudentWorld::createRegularProtestor() {
	addActor(new RegularProtester(this, 60, 60, IID_PROTESTER));
}

void StudentWorld::createSonarKit() {
	addActor(new SonarKit(this, 0, 60));
}

void StudentWorld::createWaterPool() {
	std::pair<int, int> newLocation = findNewLocation2(0, 0, 60, 60);
	addActor(new WaterPool(this, newLocation.first, newLocation.second));
}

void StudentWorld::addNewActors() {
	if (protesterSpawnTimer <= 0) {
		if (currentProtesterCount < targetProtestorCount) {
			if (getTrueByChance(std::min(90., currentLevel * 10. + 30)/100)) { // Chance of creating hardcore protestor
				createHardcoreProtestor();
			}
			else {
				createRegularProtestor();
			}
		}
		protesterSpawnTimer = std::max(25, 200 - currentLevel);
	}
	else {
		protesterSpawnTimer--;
	}

	if (getTrueByChance(1. / (currentLevel * 30 + 290))) { // Change of creating goodie
		if (getTrueByChance(1. / 5)) { // Chance of goodie being water
			createSonarKit();
		}
		else {
			createWaterPool();
		}
	}


}

void StudentWorld::updateDisplayText() {
	std::ostringstream displayText;

	using namespace std;
	displayText << "Lvl: " << right << setw(2) << currentLevel << "  "
		<< "Lives: " << setw(1) << livesLeft << "  "
		<< "Hlth: " << setw(3) << 100 * player->getHitPoints() / player->getStartingHitPoints() << "%  "
		<< "Wtr: " << setw(2) << player->getWater() << "  "
		<< "Gld: " << setw(2) << player->getGold() << "  "
		<< "Oil Left: " << setw(2) << oilLeft << "  "
		<< "Sonar: " << setw(2) << player->getSonar() << "  "
		<< "Scr: " << setw(6) << setfill('0') << currentScore;

	setGameStatText(displayText.str());
}

void StudentWorld::addToScore(int amount) {
	currentScore += amount;
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