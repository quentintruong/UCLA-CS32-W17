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
    bool obstacleAtPos(int x, int y);//checks if there is an obstacle
    int eatFood(int x, int y, int energy);
    void addFood(int x, int y, int energy);//actors call this to add a food, called by ant and grasshoppers
    void addAdult(int x, int y);
    void addAnt(int x, int y, int colony, Compiler* comp);
    
    void poisonAll(int x, int y);
    void stunAll(int x, int y);
    void biteOne(int x, int y, Actor*, int damage);
    
    
private:
    void eraseActor(int x, int y, Actor* currActor);
    
    int m_numAnts0;
    int m_numAnts1;
    int m_numAnts2;
    int m_numAnts3;
    std::vector<Actor*> m_world[VIEW_WIDTH][VIEW_HEIGHT];
    int m_ticks;
};


#endif // STUDENTWORLD_H_
