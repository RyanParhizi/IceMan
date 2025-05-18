#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"

#include <array>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
    }
	//should the base class destructor be explicitly be called? 
	~StudentWorld(){
		iceGridAction(0);
	}
    
    virtual int init()
    {
        iceGridAction(1);
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    virtual void cleanUp()
    {
    }

	void playerRemoveIce();

private:
    std::array<std::array<Ice*, 59>, 64> iceGrid{ nullptr }; //Without this nullptr initializer, weird stuff happens
	//pointer to Iceman
	//Vector to other game objects pointers (actor pointers)
    
    void iceGridAction(bool option){
        int colN = 0;
        for(auto &iceCol : iceGrid){
            int rowN = 0;
            for(auto &iceBlock : iceCol){
                if (colN == 30) {
                    int i = 0;
                };
                if(colN < 30 || colN > 33 || rowN < 4){
					if(option){
                    iceBlock = new Ice(colN, rowN);
					}
					else{
					delete iceBlock;
                    iceBlock = nullptr;
					}
                }
                rowN++;
            }
            colN++;
        }
    }
};
#endif // STUDENTWORLD_H_
