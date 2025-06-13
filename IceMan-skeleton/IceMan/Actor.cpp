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

bool Actor::processMovementInput(Direction inDir) {
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
        return moveToIfPossible(getX() + dx, getY() + dy);
    }
    return true;
}


// Agent __________________________________________________________________________________________

Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir,
    int imageID, unsigned int hitPoints)
    : Actor(world, startX, startY, startDir, true, imageID, 1, 0),
    hitPoints(hitPoints), startingHitPoints(hitPoints) {}

bool Agent::annoy(unsigned int amount) {
    hitPoints -= amount;
    if (hitPoints <= 0) {
        hitPoints = 0;
        setDead();
    }
    return true;
}
    


// Iceman _________________________________________________________________________________________

IceMan::IceMan(StudentWorld* world, int startX, int startY)
    : Agent(world, startX, startY, right, IID_PLAYER, 10) {

}
void IceMan::move() {
    if (!isAlive()) { return; }

    int key;
    getWorld()->getKey(key);

    switch (key) {
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
    case KEY_PRESS_SPACE:
        if(this->water > 0) {
            water--;
            getWorld()->addActor(new Squirt(getWorld(), getX(), getY(), getDirection()));
            break;
        }
    }
}
bool IceMan::annoy(unsigned int amount) {
    return Agent::annoy(amount);
}

void IceMan::addGold() {
    gold++;
}


// Pick up a sonar kit.
void IceMan::addSonar() {
    sonar++;
}

// Pick up water.
void IceMan::addWater() {
    water += 5;  // Add 5 squirts as specified in requirements
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
    if (!isAlive()) {
        return;
    }
    switch (m_state) {
    case State::stable:
        if (getWorld()->canActorMoveTo(this, getX(), getY()-1)) {
            m_state = State::waiting;
        }
        break;
    case State::waiting:
        ticksToWait--;
        if (ticksToWait <= 0) {
            m_state = State::falling;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        break;
    case State::falling:
        if (!processMovementInput(getDirection())) {
            setDead();
        }
        getWorld()->annoyAllNearbyActors(this, 100, 3);
        break;
    }
}

bool Boulder::canActorsPassThroughMe() const {
    return false;
}



// Squirt _________________________________________________________________________________________

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
    : Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1, 1) {
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}
void Squirt::move() {

    if (getWorld()->annoyAllNearbyActors(this, 2, 3) > 0) {
        setDead();
        return;
    }
    if (!processMovementInput(getDirection())) {
        setDead();
    }
    if (range <= 0) {
        setDead();
    }
    range--;
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
    : ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL,
                     true,  // Can be activated by Iceman
                     false, // Cannot be activated by protesters
                     false) // Starts invisible
{
    setVisible(false); // Explicitly set invisible
}

void OilBarrel::move() {
    if (!isAlive()) return;

    if (!isVisible()) {
        Actor* iceMan = getWorld()->findNearbyIceMan(this, 4.0);
        if (iceMan != nullptr) {
            setVisible(true);
            return;
        }
    }
    else {
        Actor* iceMan = getWorld()->findNearbyIceMan(this, 3.0);
        if (iceMan != nullptr) {
            setDead();
            getWorld()->playSound(SOUND_FOUND_OIL);
            getWorld()->increaseScore(1000);
            getWorld()->collectOil();  // Call the renamed method
            return;
        }
    }
}

// GoldNugget _____________________________________________________________________________________

GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY,
                      bool temporary, bool pickupByIceMan, bool visible)
    : ActivatingObject(world, startX, startY, IID_GOLD,
                      (pickupByIceMan ? SOUND_GOT_GOODIE : SOUND_PROTESTER_FOUND_GOLD),
                      pickupByIceMan, !pickupByIceMan, visible),
      m_temporary(temporary), m_pickupByIceMan(pickupByIceMan)
{
    setVisible(visible);
    if (m_temporary) {
        setTicksToLive(100); // Standard temporary lifetime
    }
}

void GoldNugget::move() {
    if (!isAlive()) return;

    if (!isVisible()) {
        // Make visible when Iceman is within 4 units
        Actor* iceMan = getWorld()->findNearbyIceMan(this, 4);
        if (iceMan != nullptr) {
            setVisible(true);
            return;
        }
    }
    else {
        if (m_pickupByIceMan) {
            Actor* iceMan = getWorld()->findNearbyIceMan(this, 3);
            if (iceMan != nullptr) {
                setDead();
                getWorld()->playSound(SOUND_GOT_GOODIE); // Hardcoded sound
                getWorld()->increaseScore(10);
                dynamic_cast<IceMan*>(iceMan)->addGold();
                return;
            }
        }
        else {
            Actor* protester = getWorld()->findNearbyPickerUpper(this, 3);
            if (protester != nullptr) {
                setDead();
                getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD); // Hardcoded sound
                dynamic_cast<Protester*>(protester)->addGold();
                return;
            }
        }
    }

    if (m_temporary) {
        decreaseTicksToLive();
        if (getTicksToLive() <= 0) {
            setDead();
        }
    }
}

// SonarKit _______________________________________________________________________________________

SonarKit::SonarKit(StudentWorld* world, int startX, int startY, int currentLevel)
    : ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, true),
      m_ticksLeft(std::max(100, 300 - 10 * currentLevel)) {
    setVisible(true);
}

void SonarKit::move() {
    if (!isAlive()) return;

    // Check if Iceman is within 3.0 units
    Actor* iceMan = getWorld()->findNearbyIceMan(this, 3);
    if (iceMan != nullptr) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        dynamic_cast<IceMan*>(iceMan)->addSonar();
        return;
    }

    // Decrease lifetime and check if it should expire
    if (--m_ticksLeft <= 0) {
        setDead();
    }
}

// WaterPool ______________________________________________________________________________________

WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
    : ActivatingObject(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE,
                     true, false, true)  // visible, pickup by IceMan only
{
    setVisible(true);
    // Calculate lifetime using max(100, 300 - 10*current_level_number)
    int ticks = std::max(100, 300 - 10 * world->getLevel());
    setTicksToLive(ticks);
}

void WaterPool::move() 
{
    if (!isAlive()) return;

    // Check if Iceman is within 3.0 units
    Actor* iceMan = getWorld()->findNearbyIceMan(this, 3);
    if (iceMan != nullptr) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        dynamic_cast<IceMan*>(iceMan)->addWater();
        return;
    }

    // Decrease lifetime and check if expired
    decreaseTicksToLive();
    if (getTicksToLive() <= 0) {
        setDead();
    }
}

