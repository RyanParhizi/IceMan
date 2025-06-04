#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <array>

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
    // Vector to other game objects pointers

    void iceGridAction(bool option); // Option 1 creates ice field. Option 0 destroys ice field.
    void deleteIceAroundPlayer(); // Deletes the ice around the player (4x4 grid)
};

#endif // STUDENTWORLD_H_
