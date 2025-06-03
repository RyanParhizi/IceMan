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
