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
