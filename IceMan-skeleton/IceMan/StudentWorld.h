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

private:
    void iceGridAction(bool option);
    void generateLevelActors();
    std::pair<int, int> findNewLocation(int x1, int y1, int x2, int y2);
   
    IceMan* player = nullptr;
    std::array<std::array<Ice*, 64>, 64> iceGrid{};
    std::vector<Actor*> levelActors;
    int currentLevel = 0;
};

#endif // STUDENTWORLD_H_







/*

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <array>
#include <vector>
#include <type_traits>

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld() { cleanUp(); }

    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:

    int keyStorage = 0;
    Iceman* player = nullptr;
    std::array<std::array<Ice*, 64>, 64> iceGrid{};
    std::vector<Actor*> levelActors;

    int currentLevel = 0;

    void iceGridAction(bool option); // Option 1 creates ice field. Option 0 destroys ice field.
    void deleteIceAroundActor(Actor* target); // Deletes the ice around the player (4x4 grid)
    void generateLevelActors();
    template<typename T>
    void placeActors(int n, int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& takenLocations);
    std::pair<int, int> generateLocation(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& takenLocations);
};

template<typename T>
void StudentWorld::placeActors(int n, int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& takenLocations) {
    for (int i = 0; i < n; i++) {
        std::pair<int, int> newLocation = generateLocation(x1, y1, x2, y2, takenLocations);
        takenLocations.push_back(newLocation);
        levelActors.push_back(new T(newLocation.first, newLocation.second));
        if constexpr (std::is_same_v<T, Ice>) { // Boulder <------------------------------------------------------------------------------------
            deleteIceAroundActor(levelActors.back());
        }
    }
}

#endif // STUDENTWORLD_H_

*/