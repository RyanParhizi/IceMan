#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth);

    // Action to perform each tick.
    virtual void move() = 0;

    // Is this actor alive?
    bool isAlive() const { return m_isAlive; }

    // Mark this actor as dead.
    void setDead() { m_isAlive = false; }

    // Annoy this actor.
    virtual bool annoy(unsigned int amt);

    // Get this actor's world
    StudentWorld* getWorld() const { return world; }

    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const { return m_canActorsPassThroughMe; }

    // Can this actor dig through Ice?
    virtual bool canDigThroughIce() const { return m_canDigThroughIce; }

    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return m_canPickThingsUp; }

    // Does this actor hunt the IceMan?
    virtual bool huntsIceMan() const { return m_huntsIceMan; }

    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const { return m_needsToBePickedUpToFinishLevel; }

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

private:
    StudentWorld* world;
    bool visible;
    bool m_isAlive = true;
    bool m_canActorsPassThroughMe = true;
    bool m_canDigThroughIce = false;
    bool m_canPickThingsUp = false;
    bool m_huntsIceMan = false;
    bool m_needsToBePickedUpToFinishLevel = false;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);

    // Pick up a gold nugget.
    virtual void addGold() = 0;

    // How many hit points does this actor have left?
    unsigned int getHitPoints() const { return hitPoints; }

    virtual bool annoy(unsigned int amount); //whyIsThisHere;
    virtual bool canPickThingsUp() const; //whyIsThisHere;
private:
    int hitPoints; //whatShouldTheDefaultBe

    void Agent::processMovementInput(Direction inDir);
};

class IceMan : public Agent
{
public:
    IceMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() const; //whyIsThisHere;

    // Pick up a sonar kit.
    void addSonar();

    // Pick up water.
    void addWater();

    // Get amount of gold
    unsigned int getGold() const { return gold; }

    // Get amount of sonar charges
    unsigned int getSonar() const { return sonar; }

    // Get amount of water
    unsigned int getWater() const { return water; }
private:
    int gold; //whatShouldTheDefaultBe
    int sonar; //whatShouldTheDefaultBe
    int water; //whatShouldTheDefaultBe
};

class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const; // whyIsThisHere

    // Set state to having given up protest
    void setMustLeaveOilField() { mustLeaveOilField = true; }

    // Set number of ticks until next move
    void setTicksToNextMove() {}
private:
    bool mustLeaveOilField;
    int ticksToNextMove;
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class Ice : public Actor
{
public:
    Ice(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const; //whyIsThisHere
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
        int soundToPlay, bool activateOnPlayer,
        bool activateOnProtester, bool initallyActive);
    virtual void move();

    // Set number of ticks until this object dies
    void setTicksToLive();
private:
    int soundToPlay;
    bool activateOnPlayer;
    bool activateOnProtester;
    bool initiallyActive;
    int ticksToLive; //whatShouldTheDefaultBe
};

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const; //whyIsThisHere
};

class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temporary);
    virtual void move();
private:
    bool temporary;
};

class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
};

#endif // ACTOR_H_



/*

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

*/