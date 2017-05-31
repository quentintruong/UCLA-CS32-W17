#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <vector>

#include <iostream>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    std::vector<Actor*> getActorsList(int x, int y);

private:
    void removeActorFromGrid(int x, int y, Actor* currActor);
    
    std::vector<Actor*> m_world[VIEW_WIDTH][VIEW_HEIGHT];
    int m_ticks;
};


#endif // STUDENTWORLD_H_
