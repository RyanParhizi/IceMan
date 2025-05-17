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
    
    ~Actor() {}

    virtual void doSomething () = 0;
    
    void setAlive(bool alive) {
        isAlive = alive;
    }

private:
    bool isAlive = true;
};

class Ice : public Actor {
public:
    Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}
    
    void doSomething() override {}
};

class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : Actor(imageID, startX, startY, dir, size, depth) {}

    void annoy(int damage) {
        hitPoints -= damage;
    }

private:
    int hitPoints;
};

class Iceman : public Person {
public:
    Iceman()
    : Person(IID_PLAYER, 30, 60, ...) // TO_DO
private:

};


#endif // ACTOR_H_