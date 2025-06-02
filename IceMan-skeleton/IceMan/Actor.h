#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void setAlive(bool alive_);
    bool isAlive();
    
private:
    bool alive = true;
};

class Ice : public Actor {
public:
    Ice(int startX, int startY);
    virtual ~Ice();
    void doSomething() override;

};

class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    void annoy(int damage);

protected:
    void processMovementInput(Direction inDir);

private:
    int hitPoints = 0;
    int coordClamp(int&& coord);

};

class Iceman : public Person {
public:
    Iceman(const int& keyRef);
    virtual ~Iceman();
    void doSomething() override;

private:
    const int& key;
    int hitPoints;
    int squirtCount;
    int sonarCount;
    int goldCount;

};

class Goodies : public Actor {

public:
    Goodies(int imageID, int startX, int startY, unsigned int depth = 2) //pass reference to the iceman, this way we can keep track of location and increment inventory
    : Actor(imageID, startX, startY, right, 1.0, depth)
    {
        state = true;
    }

    virtual ~Goodies();
    virtual void doSomething() override = 0; // in goodies do the collision check with the location of the goodie object and the location of iceman, if collision, setvisble to false and setisalive to false, 
    bool isVisibleToIceman();
    void setVisibleToIceman(bool s);

private:
    bool state;
};

class GoldNugget : public Goodies {
public:
    GoldNugget(int startX, int startY, bool temporary, int lifetime = 100)
    : Goodies(IID_GOLD, startX, startY, 2);

    virtual ~GoldNugget();
    void doSomething() override;

#endif // ACTOR_H_
