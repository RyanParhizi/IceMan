#include "Actor.h"
#include "StudentWorld.h"
#include <chrono> 

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
    setVisible(true);
}

Actor::~Actor() {}

void Actor::setAlive(bool alive_) {
    alive = alive_;
}


bool Actor::isAlive(){
    return alive; 
}

/*----------------------------------Ice---------------------------------------*/

Ice::Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}

Ice::~Ice() {}

void Ice::doSomething() {}

/*----------------------------------Person------------------------------------*/

Person::Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth )
    : Actor(imageID, startX, startY, dir, size, depth) {}

Person::~Person() {}

void Person::annoy(int damage) {
    hitPoints -= damage;
}

void Person::processMovementInput(Direction inDir) {
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
        moveTo(coordClamp(getX() + dx), coordClamp(getY() + dy));
    }
    return;
}

int Person::coordClamp(int&& coord) {
    if (coord < 0) { coord = 0; }
    if (coord > 60) { coord = 60; }
    return coord;
}

/*----------------------------------Iceman-------------------------------------*/

Iceman::Iceman(const int& keyRef)
    : Person(IID_PLAYER, 30, 60, right, 1.0, 0), key(keyRef) {

    hitPoints = 10; 
    squirtCount = 5;
    sonarCount = 1;
    goldCount = 0;
}

Iceman::~Iceman() {}

void Iceman::doSomething()  {
    if (!isAlive()) { return; }

    switch(key) {
    case KEY_PRESS_RIGHT:
        processMovementInput(right);
        break;
    case KEY_PRESS_LEFT:
        processMovementInput(left);
        break;
    case KEY_PRESS_UP:
        processMovementInput(up);
        break;
    case KEY_PRESS_DOWN:
        processMovementInput(down);
        break;
    }
}

/*----------------------------------Goodies------------------------------------*/

Goodies::Goodies(int imageID, int startX, int startY, Iceman& playerRef, unsigned int depth)
    : Actor(imageID, startX, startY, right, 1.0, depth), player(playerRef) {
    permeance = true;
}

Goodies::~Goodies() {}

bool Goodies::ispermanent() const {
    return permeance;
}

void Goodies::setVisibleToIceman(bool s) {
    permeance = s;
    setVisible(s);
}

void Goodies::startLifetimeTimer() {
    startTime = std::chrono::steady_clock::now();
}

bool Goodies::hasLifetimeExpired() const 
{
    if (ispermanent()) {
        return false; // Permanent objects don't expire
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    
    return elapsed >= lifetime;
}

/*----------------------------------GoldNugget------------------------------------*/

//GoldNugget::GoldNugget(int startX, int startY, bool temporary, int lifetime = 100, bool permeance = true)
//: Goodies(IID_GOLD, startX, startY, 2) {}

GoldNugget::~GoldNugget() {}

void GoldNugget::doSomething() {}

/*

--waterPuddle--

*/

/*

--oilBarrel--

*/

/*

--sonarKit--

*/
