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
        
        // iceGrid[player->getX()][player->getY()] = new Ice(player->getX(), player->getY());
        iceGrid[player->getX()][player->getY()] = nullptr;
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move() {
        if(!getKey(keyStorage)){
            keyStorage = 0;
        }
        
        player->doSomething();
        // deleteIceByIndex(player->getX(), player->getY());

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
    std::array<std::array<Ice*, 60>, 64> iceGrid{ nullptr };
	//Vector to other game objects pointers

    void iceGridAction(bool option){ // Option 1 creates ice field. Option 0 destroys ice field.
        int colN = 0;
        for(auto &iceCol : iceGrid){
            int rowN = 0;
            for(auto &iceBlock : iceCol){
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

     bool isIce(int x, int y){
         if(iceGrid[x][y] == NULL){
             return 0; // there is no ice at the index
         }
         else{
             return 1; // there is ice at the index
         }
     }

     void deleteIceByIndex(int x, int y){
             for(int i = 0; i < 4; i++){
                 for(int j = 0; j < 4; j++){
                    delete iceGrid[x + i][y - j];
                    iceGrid[x + i][y - j] = nullptr;
                 }
             }
     return;
     }
    
};

#endif // STUDENTWORLD_H_
