#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(true);
    }
    
    virtual ~Actor() {}

    virtual void doSomething() {}
    
    void setAlive(bool alive_) {
        alive = alive_;
    }
    bool isAlive(){
        return alive; 
    }
    

private:
    bool alive = true;
};

class Ice : public Actor {
public:
    Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}
    
    virtual ~Ice() {}

    void doSomething() override {}
};

class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : Actor(imageID, startX, startY, dir, size, depth) {}

    virtual ~Person() {}

    void annoy(int damage) {
        hitPoints -= damage;
    }

protected:
    
    void processMovementInput(Direction inDir) {
        if (getDirection() != inDir) {
            setDirection(inDir);
        }
        else {
            int dx = 0;
            int dy = 0;
            switch (inDir) {
            case right:
                dx = 1;
            case left:
                dx = -1;
            case up:
                dy = 1;
            case down:
                dy = -1;
            }
            moveTo(getX() + dx, getY() + dy);
        }
        return;
    }

private:
    int hitPoints = 0;
};

class Iceman : public Person {
public:
    Iceman(const int& keyRef)
    : Person(IID_PLAYER, 30, 60, right, 1.0, 0), key(keyRef) {

     hitPoints = 10; 
     squirtCount = 5;
     sonarCount = 1;
     goldCount = 0;
    }

    virtual ~Iceman() {}

    void doSomething() override {
        if (!isAlive()) { return; }

        switch(key) {
        case KEY_PRESS_RIGHT:
            processMovementInput(right);
        case KEY_PRESS_LEFT:
            processMovementInput(left);
        case KEY_PRESS_UP:
            processMovementInput(up);
        case KEY_PRESS_DOWN:
            processMovementInput(down);
        }
    }

private:

    const int& key;

    int hitPoints;
    int squirtCount;
    int sonarCount;
    int goldCount;
};

class Goodies : public Actor {

public:
    Goodies(int imageID, int startX, int startY, unsigned int depth = 2)
    : Actor(imageID, startX, startY, right, 1.0, depth)
    {
        state = true;
    }

    virtual ~Goodies() {}

    virtual void doSomething() override = 0;

    bool isVisibleToIceman() const {
        return state;
    }

    void setVisibleToIceman(bool s) {
        state = s;
        setVisible(s);
    }

private:
    bool state;
};

class GoldNugget : public Goodies {
public:
    GoldNugget(int startX, int startY, bool temporary, int lifetime = 100)
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

    virtual ~GoldNugget() {}

    void doSomething() override
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

private:
    bool m_temporary;
    int m_lifetime;
};

#endif // ACTOR_H_