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
    bool permeance; //this determines if the object can vanish if not collected after some elapsed time
    Iceman& player;
    //add a function here to determined time elapsed for items to vanish, sonar kit and water puddle. 
    //check expire function, access the permeance variable, check the current time against its expiration data, change isalive state 
};

class GoldNugget : public Goodies {
public:
    GoldNuggetGoldNugget(int startX, int startY, bool temporary, int lifetime = 100, bool permeance = true);
    virtual ~GoldNugget();
    void doSomething() override;
};

class waterPuddle : public Goodies {

};

class oilBarrel : public goodies {

};

class sonarKit : public goodies {

};

#endif // ACTOR_H_
