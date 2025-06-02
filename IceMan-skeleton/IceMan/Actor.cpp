#include "Actor.h"
#include "StudentWorld.h"

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

Ice::Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}

Ice::~Ice() {}

void Ice::doSomething() {}

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

    Goodies::~Goodies() {}

    bool Goodies::ispermanent() const {
        return state;
    }

    void Goodies::setVisibleToIceman(bool s) {
        state = s;
        setVisible(s);
    }

    GoldNugget::GoldNugget(int startX, int startY, bool temporary, int lifetime = 100)
    : Goodies(IID_GOLD, startX, startY, 2)
    {
        m_temporary = temporary;
        m_lifetime = lifetime;
        if (!temporary) {
            setVisible(false); 
        } else {
            setVisible(true);
        }
    }    

    virtual GoldNugget::~GoldNugget();

    void GoldNugget::doSomething() override
    {
        if (!isAlive()) {
            return;
        }

        if (m_temporary) {
            if (--m_lifetime <= 0) {
                setAlive(false);
                return;
            }

            // Add logic to detect if Proteste picks up the gold
            // If so, give score, setAlive(false), play sound
        } else {
            // Add logic to check if Iceman is close enough
            // if so give gold to player, increase score, setAlive(false), play sound
        }
    }
