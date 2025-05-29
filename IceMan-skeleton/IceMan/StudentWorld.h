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
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir) {

    }

    ~StudentWorld() {
        cleanUp();
	}
    
    virtual int init() {
        player = new Iceman(keyStorage);
        iceGridAction(1);
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move() {
        if(!getKey(keyStorage)){
            keyStorage = 0;
        }
        
        player->doSomething();
        deleteIceAroundPlayer();

        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp() {
        delete player;
        player = nullptr;
        iceGridAction(0);
    }

private:
    
    int keyStorage = 0;
    Iceman* player = nullptr;
    std::array<std::array<Ice*, 64>, 64> iceGrid{ nullptr };
	//Vector to other game objects pointers

    void iceGridAction(bool option){ // Option 1 creates ice field. Option 0 destroys ice field.
        int colN = 0;
        for(auto &iceCol : iceGrid){
            int rowN = 0;
            for(auto &iceBlock : iceCol){
                if((colN < 30 || colN > 33 || rowN < 4) && rowN < 60){
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

    void deleteIceAroundPlayer(){
        int px = player->getX();
        int py = player->getY();
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                   delete iceGrid[px + i][py + j];
                   iceGrid[px + i][py + j] = nullptr;
                }
            }
            std::cout << std::endl;
    return;
    }
    
};

#endif // STUDENTWORLD_H_
