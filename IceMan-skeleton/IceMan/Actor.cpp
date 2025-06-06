#include "Actor.h"
#include "StudentWorld.h"

// Actor __________________________________________________________________________________________

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir,
    bool visible, int imageID, double size, int depth)
    : GraphObject(imageID, startX, startY, startDir, size, depth),
    world(world), visible(visible) {
    setVisible(visible);
}


// Annoy this actor.
bool Actor::annoy(unsigned int amt) {
    return false;
}

// Move this actor to x,y if possible, and return true; otherwise,
// return false without moving.
bool Actor::moveToIfPossible(int x, int y) {
    if (world->canActorMoveTo(this, x, y)) {
        moveTo(x, y);
        return true;
    }
    else {
        return false;
    }
}


// Agent __________________________________________________________________________________________

Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir,
    int imageID, unsigned int hitPoints)
    : Actor(world, startX, startY, startDir, true, imageID, 1, 0), hitPoints(hitPoints) {

}

bool Agent::annoy(unsigned int amount) {
    return false;
} //whyIsThisHere;
bool Agent::canPickThingsUp() const {
    return false;
} //whyIsThisHere;

void Agent::processMovementInput(Direction inDir) {
    if (getDirection() != inDir) {
        setDirection(inDir);
    }
    else {
        int dx = 0;
        int dy = 0;
        switch (inDir) {
        case right:
            dx = 1;
            break;
        case left:
            dx = -1;
            break;
        case up:
            dy = 1;
            break;
        case down:
            dy = -1;
            break;
        }
        moveToIfPossible(getX() + dx, getY() + dy);
    }
}

// Iceman _________________________________________________________________________________________

IceMan::IceMan(StudentWorld* world, int startX, int startY)
    : Agent(world, startX, startY, right, IID_PLAYER, 10) {

}
void IceMan::move() {

}
bool IceMan::annoy(unsigned int amount) {
    return false;
}
void IceMan::addGold() {

}
bool IceMan::canDigThroughIce() const {
    return false;
} //whyIsThisHere;


// Pick up a sonar kit.
void IceMan::addSonar() {

}

// Pick up water.
void IceMan::addWater() {

}


// Protester ______________________________________________________________________________________

Protester::Protester(StudentWorld* world, int startX, int startY, int imageID,
    unsigned int hitPoints, unsigned int score)
    : Agent(world, startX, startY, left, imageID, hitPoints) {

}
void Protester::move() {

}
bool Protester::annoy(unsigned int amount) {
    return false;
}
void Protester::addGold() {

}
bool Protester::huntsIceMan() const {
    return false;
} // whyIsThisHere


// RegularProtester _______________________________________________________________________________

RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, IID_PROTESTER, 5, 100) {

}
void RegularProtester::move() {

}
void RegularProtester::addGold() {

}


// HardcoreProtester _______________________________________________________________________________

HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 20, 250) {

}
void HardcoreProtester::move() {

}
void HardcoreProtester::addGold() {

}


// Ice ____________________________________________________________________________________________

Ice::Ice(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { //Curious why GraphObject doesn't work.

}
void Ice::move() {

}


// Boulder ________________________________________________________________________________________

Boulder::Boulder(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, down, true, IID_BOULDER, 1, 1) {

}
void Boulder::move() {

}
bool Boulder::canActorsPassThroughMe() const {
    return false;
}//whyIsThisHere


// Squirt _________________________________________________________________________________________

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
    : Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1, 1) {

}
void Squirt::move() {

}


// ActivatingObject _______________________________________________________________________________

ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
    int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive)
    : Actor(world, startX, startY, right, true, imageID, 1, 2),
    soundToPlay(soundToPlay), activateOnPlayer(activateOnPlayer),
    activateOnProtester(activateOnProtester), initiallyActive(initiallyActive){

}
void ActivatingObject::move() {

}

// Set number of ticks until this object dies
void ActivatingObject::setTicksToLive() {

}


// OilBarrel ______________________________________________________________________________________

OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
    :ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL,
        true, false, false) {

}
void OilBarrel::move() {

}
bool OilBarrel::needsToBePickedUpToFinishLevel() const {
    return false;
} //whyIsThisHere


// GoldNugget _____________________________________________________________________________________

GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temporary)
    :ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true,
    true, true), temporary(temporary) {

}
void GoldNugget::move() {

}


// SonarKit _______________________________________________________________________________________

SonarKit::SonarKit(StudentWorld* world, int startX, int startY)
    :ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE,
        true, false, true) {

}
void SonarKit::move() {

}


// WaterPool ______________________________________________________________________________________

WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
    :ActivatingObject(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE,
        true, false, true) {

}
void WaterPool::move() {

}


//Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
//    : GraphObject(imageID, startX, startY, dir, size, depth) {
//    setVisible(true);
//}
//
//Actor::~Actor() {}
//
//void Actor::setAlive(bool alive_) {
//    alive = alive_;
//}
//
//
//bool Actor::isAlive(){
//    return alive; 
//}

/*----------------------------------Ice---------------------------------------*/

//Ice::Ice(int startX, int startY)
//    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}
//
//Ice::~Ice() {}
//
//void Ice::doSomething() {}

/*----------------------------------Person------------------------------------*/

//Person::Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth )
//    : Actor(imageID, startX, startY, dir, size, depth) {}
//
//Person::~Person() {}
//
//void Person::annoy(int damage) {
//    hitPoints -= damage;
//}
//
//void Person::processMovementInput(Direction inDir) {
//    if (getDirection() != inDir) {
//        setDirection(inDir);
//    }
//    else {
//        int dx = 0;
//        int dy = 0;
//        switch (inDir) {
//        case right:
//            dx = 1;
//            break;
//        case left:
//            dx = -1;
//            break;
//        case up:
//            dy = 1;
//            break;
//        case down:
//            dy = -1;
//            break;
//        }
//        moveTo(coordClamp(getX() + dx), coordClamp(getY() + dy));
//    }
//    return;
//}
//
//int Person::coordClamp(int&& coord) {
//    if (coord < 0) { coord = 0; }
//    if (coord > 60) { coord = 60; }
//    return coord;
//}

/*----------------------------------Iceman-------------------------------------*/

//Iceman::Iceman(const int& keyRef)
//    : Person(IID_PLAYER, 30, 60, right, 1.0, 0), key(keyRef) {
//
//    hitPoints = 10; 
//    squirtCount = 5;
//    sonarCount = 1;
//    goldCount = 0;
//}
//
//Iceman::~Iceman() {}
//
//void Iceman::doSomething()  {
//    if (!isAlive()) { return; }
//
//    switch(key) {
//    case KEY_PRESS_RIGHT:
//        processMovementInput(right);
//        break;
//    case KEY_PRESS_LEFT:
//        processMovementInput(left);
//        break;
//    case KEY_PRESS_UP:
//        processMovementInput(up);
//        break;
//    case KEY_PRESS_DOWN:
//        processMovementInput(down);
//        break;
//    }
//}

/*----------------------------------Goodies------------------------------------*/

//Goodies::Goodies(int imageID, int startX, int startY, Iceman& playerRef, unsigned int depth)
//    : Actor(imageID, startX, startY, right, 1.0, depth), player(playerRef) {
//    permeance = true;
//}
//
//Goodies::~Goodies() {}
//
//bool Goodies::ispermanent() const {
//    return permeance;
//}
//
//void Goodies::setVisibleToIceman(bool s) {
//    permeance = s;
//    setVisible(s);
//}

/*----------------------------------GoldNugget------------------------------------*/

//GoldNugget::GoldNugget(int startX, int startY, bool temporary, int lifetime = 100, bool permeance = true)
//: Goodies(IID_GOLD, startX, startY, 2) {}

//GoldNugget::~GoldNugget() {}
//
//void GoldNugget::doSomething() {}

/*

--waterPuddle--

*/

/*

--oilBarrel--

*/

/*

--sonarKit--

*/
