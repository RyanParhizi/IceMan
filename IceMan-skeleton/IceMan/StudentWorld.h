#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include <array>
#include <vector>

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    void increaseScore(int points) { currentScore += points; }

    int getLevel() const { return currentLevel; }

    void collectOil() { m_oilBarrelsLeft--; }  // Renamed to be more clear
    int getOilLeft() const { return m_oilBarrelsLeft; }
    // Add an actor to the world.
    void addActor(Actor* a);

    // Clear a 4x4 region of Ice.
    void clearIce(int x, int y);

    // Can actor move to x,y?
    bool canActorMoveTo(Actor* a, int x, int y) const;

    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);

    // If the IceMan is within radius of a, return a pointer to the
    // IceMan, otherwise null.
    Actor* findNearbyIceMan(Actor* a, int radius) const;

    // If at least one actor that can pick things up is within radius of a,
    // return a pointer to one of them, otherwise null.
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;

    // Annoy the IceMan.
    void annoyIceMan();

    // Give IceMan some sonar charges.
    void giveIceManSonar();

    // Give IceMan some water.
    void giveIceManWater();

    // Is the Actor a facing toward the IceMan?
    bool facingTowardIceMan(Actor* a) const;

    // If the Actor a has a clear line of sight to the IceMan, return
    // the direction to the IceMan, otherwise GraphObject::none.
    GraphObject::Direction lineOfSightToIceMan(Actor* a) const;

    // Return whether the Actor a is within radius of IceMan.
    bool isNearIceMan(Actor* a, int radius) const;

    // Determine the direction of the first move a quitting protester
    // makes to leave the oil field.
    GraphObject::Direction determineFirstMoveToExit(int x, int y);

    // Determine the direction of the first move a hardcore protester
    // makes to approach the IceMan.
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);

    // Returns Iceman's current location
    void getIceManLocation(int& x, int& y) const;

    // Returns the number of steps from (startX, startY) to (endX, endY) using BFS, or -1 if unreachable
    int stepsToTarget(int startX, int startY, int endX, int endY) const;

    // Returns the first direction to move from (startX, startY) toward (endX, endY) along the shortest path
    GraphObject::Direction determineFirstMoveToTarget(int startX, int startY, int endX, int endY) const;

    std::array<std::array<Ice*, 64>, 64>& getIceGrid() { return iceGrid; }

    void oilBarrelCreated() { m_oilBarrelsLeft++; }
    bool allIceAtCoords(int x, int y, bool mod) const;


private:
    void iceGridAction(bool option);
    void generateLevelActors();
    std::pair<int, int> findNewLocation(int x1, int y1, int x2, int y2, bool underIce);

    bool getTrueByChance(double chance);
    void createHardcoreProtestor();
    void createRegularProtestor();
    void createSonarKit();
    void createWaterPool();
    void addNewActors();

    void updateDisplayText();

    void addToScore(int amount);

    GraphObject::Direction pathFind(int x1, int y1, int x2, int y2);
   
    IceMan* player = nullptr;
    std::array<std::array<Ice*, 64>, 64> iceGrid{};
    std::vector<Actor*> levelActors;
    int currentLevel = 0;
    int livesLeft = 3;
    int currentScore = 0;
    int m_oilBarrelsLeft = 0;

    int protesterSpawnTimer = 0;
    int currentProtesterCount = 0;
    int targetProtestorCount = 0;
};

#endif // STUDENTWORLD_H_