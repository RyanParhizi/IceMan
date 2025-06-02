#include "StudentWorld.h"
#include <string>

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
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	if (!getKey(keyStorage)) {
		keyStorage = 0;
	}
	player->doSomething();
	//iterate over the actor vector (contains boulders, squirts, gun, goodies, etc) and ruch each of the dosomethings. each actor has access to the iceman
	deleteIceAroundPlayer();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	delete player;
	player = nullptr;
	iceGridAction(0);
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

void StudentWorld::deleteIceAroundPlayer() {
	int px = player->getX();
	int py = player->getY();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			delete iceGrid[px + i][py + j];
			iceGrid[px + i][py + j] = nullptr;
		}
	}
}