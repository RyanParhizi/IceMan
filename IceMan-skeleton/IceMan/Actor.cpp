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
void IceMan::doSomething() {
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
        if(water > 0) {
            sprayWater();
        }
        break;
    case KEY_PRESS_TAB:
        if (gold > 0) {
            getWorld()->addActor(new GoldNugget(getWorld(), getX(), getY(), true, false, true));
            gold--;
        }
        break;
    case 'z': case 'Z':
        if (sonar > 0) {
            getWorld()->playSound(SOUND_SONAR);
            getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
            sonar--;
        }
        break;
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
    water = 5;  // Add 5 squirts as specified in requirements
}

void IceMan::sprayWater() // Spawn position looks odd compared to demo.
{
    water--;
    int dx = 0; // This is copied and modifyed from the processMovementInput() function. Consider making this a function itself.
    int dy = 0;
    switch (getDirection()) {
    case right:
        dx = 4;
        break;
    case left:
        dx = -4;
        break;
    case up:
        dy = 4;
        break;
    case down:
        dy = -4;
        break;
    }
    getWorld()->addActor(new Squirt(getWorld(), getX() + dx, getY() + dy, getDirection()));
}


// Protester ______________________________________________________________________________________

Protester::Protester(StudentWorld* world, int startX, int startY, int imageID,
    unsigned int hitPoints, unsigned int score)
    : Agent(world, startX, startY, left, imageID, hitPoints),
      mustLeaveOilField(false), m_score(score)
{
    restTicks = std::max(0, 3 - world->getLevel() / 4);
    numSquaresToMoveInCurrentDirection = 8 + rand() % 53;
    shoutCooldown = 0;
    turnCooldown = 0;
}

void Protester::doSomething() {
    if (!isAlive()) return;

    // 1. Resting state
    if (restTicks > 0) {
        restTicks--;
        return;
    }
    restTicks = std::max(0, 3 - getWorld()->getLevel() / 4);

    // 2. Leave-the-oil-field state
    if (mustLeaveOilField) {
        if (getX() == 60 && getY() == 60) {
            setDead();
            return;
        }
        // Move one step toward exit using BFS direction
        GraphObject::Direction dir = getWorld()->determineFirstMoveToExit(getX(), getY());
        processMovementInput(dir); // This should be a viable substitute for the code below
        //int dx = 0, dy = 0;
        //switch (dir) {
        //    case left: dx = -1; break;
        //    case right: dx = 1; break;
        //    case up: dy = 1; break;
        //    case down: dy = -1; break;
        //    case none: break; // <-- this is fine
        //    // default: break; // optional
        //}
        //moveToIfPossible(getX() + dx, getY() + dy);
        return;
    }

    // 3. Shout cooldown
    if (shoutCooldown > 0) shoutCooldown--;

    // 4. Shout at Iceman if within 4 units and facing
    Actor* iceman = getWorld()->findNearbyIceMan(this, 4);
    if (iceman) {
        int dx = iceman->getX() - getX();
        int dy = iceman->getY() - getY();
        bool facing = false;
        switch (getDirection()) {
            case left:  facing = (dx < 0 && dy == 0); break;
            case right: facing = (dx > 0 && dy == 0); break;
            case up:    facing = (dy > 0 && dx == 0); break;
            case down:  facing = (dy < 0 && dx == 0); break;
            default: break;
        }
        if (facing && shoutCooldown == 0) {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            iceman->annoy(2);
            shoutCooldown = 15;
            return;
        }
    }

    // 5(hardcore). Seek out IceMan
    if (hardcoreFunctionality()) {
        return;
    }

    // 5. Line of sight to Iceman, more than 4 units away, and path is clear
    GraphObject::Direction losDir = getWorld()->lineOfSightToIceMan(this);
    if (losDir != GraphObject::none) {
        Actor* iceman = getWorld()->findNearbyIceMan(this, 100);
        double dist = std::hypot(getX() - iceman->getX(), getY() - iceman->getY());
        if (dist > 4.0) {
            setDirection(losDir);
            processMovementInput(losDir); // This should be a viable substitute for the code below
            //int dx = 0, dy = 0;
            //switch (losDir) {
            //    case left: dx = -1; break;
            //    case right: dx = 1; break;
            //    case up: dy = 1; break;
            //    case down: dy = -1; break;
            //    default: break;
            //}
            //
            //moveToIfPossible(getX() + dx, getY() + dy);
            numSquaresToMoveInCurrentDirection = 0;
            return;
        }
    }

    // 6. Perpendicular turn every 200 non-resting ticks if possible
    if (turnCooldown > 0)
        turnCooldown--;
    else {
        std::vector<GraphObject::Direction> perpendiculars;
        if (getDirection() == left || getDirection() == right) {
            if (getWorld()->canActorMoveTo(this, getX(), getY() + 1)) perpendiculars.push_back(up);
            if (getWorld()->canActorMoveTo(this, getX(), getY() - 1)) perpendiculars.push_back(down);
        } else {
            if (getWorld()->canActorMoveTo(this, getX() + 1, getY())) perpendiculars.push_back(right);
            if (getWorld()->canActorMoveTo(this, getX() - 1, getY())) perpendiculars.push_back(left);
        }
        if (!perpendiculars.empty()) {
            setDirection(perpendiculars[rand() % perpendiculars.size()]);
            numSquaresToMoveInCurrentDirection = 8 + rand() % 53;
            turnCooldown = 200;
        }
    }

    // 7. Random walk if needed
    if (numSquaresToMoveInCurrentDirection <= 0) {
        std::vector<GraphObject::Direction> dirs;
        if (getWorld()->canActorMoveTo(this, getX() + 1, getY())) dirs.push_back(right);
        if (getWorld()->canActorMoveTo(this, getX() - 1, getY())) dirs.push_back(left);
        if (getWorld()->canActorMoveTo(this, getX(), getY() + 1)) dirs.push_back(up);
        if (getWorld()->canActorMoveTo(this, getX(), getY() - 1)) dirs.push_back(down);
        if (!dirs.empty()) {
            setDirection(dirs[rand() % dirs.size()]);
            numSquaresToMoveInCurrentDirection = 8 + rand() % 53;
        }
    }

    // 8. Try to move in current direction
    if(processMovementInput(getDirection())) {
    
    //int dx = 0, dy = 0;
    //switch (getDirection()) {
    //    case right: dx = 1; break;
    //    case left: dx = -1; break;
    //    case up: dy = 1; break;
    //    case down: dy = -1; break;
    //    default: break;
    //}
    //if (getWorld()->canActorMoveTo(this, getX() + dx, getY() + dy)) {
    //    moveTo(getX() + dx, getY() + dy);
        numSquaresToMoveInCurrentDirection--;
    } else {
        numSquaresToMoveInCurrentDirection = 0;
    }
}

bool Protester::annoy(unsigned int amount) {
    if (!isAlive() || mustLeaveOilField) return false;
    hitPoints -= amount;
    if (hitPoints <= 0) {
        hitPoints = 0;
        mustLeaveOilField = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        restTicks = 0;
        if (amount == 2) { // Water Squirt
            getWorld()->increaseScore(100);
        }
        else { // Boulder
            getWorld()->increaseScore(500);

        }
    } else {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        restTicks = std::max(50, 100 - getWorld()->getLevel() * 10);
    }
    return true;
}

void Protester::addGold() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    mustLeaveOilField = true;
}


// RegularProtester _______________________________________________________________________________

RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, IID_PROTESTER, 5, 100) {

}
void RegularProtester::doSomething() {
    Protester::doSomething();
}

void RegularProtester::addGold() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    mustLeaveOilField = true;
}


// HardcoreProtester _______________________________________________________________________________

HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 20, 250) // Adjust hitpoints/score as needed
{
}

void HardcoreProtester::doSomething() {
    Protester::doSomething();
}

void HardcoreProtester::addGold() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    restTicks = std::max(50, 100 - getWorld()->getLevel() * 10);
}

bool HardcoreProtester::hardcoreFunctionality()
{
    static int M = 16 + getWorld()->getLevel() * 2;

    GraphObject::Direction dir = getWorld()->determineFirstMoveToIceMan(getX(), getY(), M);

    if (dir == GraphObject::none) {
        return false;
    }
    else {
        processMovementInput(dir);
        return true;
    }
}


// Ice ____________________________________________________________________________________________

Ice::Ice(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { //Curious why GraphObject doesn't work.

}
void Ice::doSomething() {

}


// Boulder ________________________________________________________________________________________

Boulder::Boulder(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, down, true, IID_BOULDER, 1, 1) {

}
void Boulder::doSomething() {
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
    if (!getWorld()->allIceAtCoords(getX(), getY(), false)) {
        setDead();
    }
}
void Squirt::doSomething() {


    if (spawnTickDelay) {
        spawnTickDelay = false;
    }
    else {
        if (getWorld()->annoyAllNearbyActors(this, 2, 3) > 0) {
            setDead();
            return;
        }
        if (!processMovementInput(getDirection())) {
            setDead();
        }
    }
    if (range <= 0) {
        setDead();
    }
    range--;
}


// ActivatingObject _______________________________________________________________________________

ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
    int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive)
    : Actor(world, startX, startY, right, false, imageID, 1, 2),
    soundToPlay(soundToPlay), activateOnPlayer(activateOnPlayer),
    activateOnProtester(activateOnProtester), initiallyActive(initallyActive){

}
void ActivatingObject::doSomething() {

}

// Set number of ticks until this object dies
void ActivatingObject::setTicksToLive() {

}


// OilBarrel ______________________________________________________________________________________

OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
    :ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL,
        true, false, false) {
    getWorld()->oilBarrelCreated();
}

void OilBarrel::doSomething() {
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
    if (m_temporary) {
        setVisible(visible);
        setTicksToLive(100); // Standard temporary lifetime
    }
}

void GoldNugget::doSomething() {
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

void SonarKit::doSomething() {
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

void WaterPool::doSomething() 
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
