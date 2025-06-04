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

    Goodies(int imageID, int startX, int startY, Iceman& playerRef, unsigned int depth = 2);
    virtual ~Goodies();
    virtual void doSomething() override = 0;
    bool ispermanent() const;
    void setVisibleToIceman(bool s);

protected: //this way the derived classes can access these elements
    bool state;
    Iceman& player;
};

class GoldNugget : public Goodies {
public:
    GoldNugget(int startX, int startY, bool temporary, int lifetime = 100);;
    virtual ~GoldNugget();
    void doSomething() override;
};;

#endif // ACTOR_H_
