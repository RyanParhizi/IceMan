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

#endif // ACTOR_H_
