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

private:
    int hitPoints = 0;

    int coordClamp(int&& coord) {
        if (coord < 0) { coord = 0; }
        if (coord > 60) { coord = 60; }
        return coord;
    }
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

private:

    const int& key;

    int hitPoints;
    int squirtCount;
    int sonarCount;
    int goldCount;
};


#endif // ACTOR_H_