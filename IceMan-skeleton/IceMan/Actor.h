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
    virtual bool canActorsPassThroughMe() const { return true; }

    // Can this actor dig through Ice?
    virtual bool canDigThroughIce() const { return false; }

    // Can this actor pick items up?
    virtual bool canPickThingsUp() const { return false; }

    // Does this actor hunt the IceMan?
    virtual bool huntsIceMan() const { return false; }

    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const { return false; }

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

    virtual bool canHurtIceMan() const { return true; }

protected:
    StudentWorld* world;
    bool visible;
    bool m_isAlive = true;

    bool processMovementInput(Direction inDir);
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
    unsigned int getStartingHitPoints() const { return startingHitPoints; }

    virtual bool annoy(unsigned int amount); //whyIsThisHere;
    virtual bool canPickThingsUp() const { return true; }

protected:
    int hitPoints;
    int startingHitPoints;

};

class IceMan : public Agent
{
public:
    IceMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() { return true; }

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
    int gold = 0;
    int sonar = 1;
    int water = 5;

    void sprayWater();
};

class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const { return true; }

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
    enum class State { stable, waiting, falling };
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
private:
    State m_state = State::stable;
    int ticksToWait = 30;
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();

    virtual bool canHurtIceMan() const { return false; }

private:
    int range = 4;
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

    void setTicksToLive(int ticks) { ticksToLive = ticks; }
    int getTicksToLive() const { return ticksToLive; }
    void decreaseTicksToLive() { ticksToLive--; }

private:
    int soundToPlay;
    bool activateOnPlayer;
    bool activateOnProtester;
    bool initiallyActive;
    int ticksToLive; //whatShouldTheDefaultBe
};

class OilBarrel : public ActivatingObject {
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const { return true; }
};

class GoldNugget : public ActivatingObject {
public:
    GoldNugget(StudentWorld* world, int startX, int startY, 
              bool temporary, bool pickupByIceMan, bool visible);
    virtual void move();

private:
    bool m_temporary;
    bool m_pickupByIceMan;
};

class SonarKit : public ActivatingObject {
public:
    SonarKit(StudentWorld* world, int startX, int startY, int currentLevel);
    virtual void move();

private:
    int m_ticksLeft;
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
};

#endif // ACTOR_H_