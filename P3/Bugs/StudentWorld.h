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

class StudentWorld : public GameWorld {
public:
	StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    int eatFood(int x, int y, int energy);
    void poisonAll(int x, int y);
    void stunAll(int x, int y);
    void biteOne(int x, int y, Actor*, int damage);
    void biteEnemy(int x, int y, int colony);
    
    void addFood(int x, int y, int energy);
    void addPheromone(int x, int y, int colony);
    void addAnt(int x, int y, int colony, Compiler* comp);
    void addAdult(int x, int y);
    
    bool hasObstacle(int x, int y);
    bool hasFood(int x, int y);
    bool hasPheromone(int x, int y, int colony);
    bool hasEnemy(int x, int y, int colony);
    bool hasHill(int x, int y, int colony);
    bool hasDanger(int x, int y, int colony);
    Compiler* getCompiler(int colony);
    
private:
    void eraseActor(int x, int y, Actor* currActor);
    int getNumberOfAntsFor(int colony);
    int getWinningAntColony();
    bool weHaveAWinningAnt();
    
    int m_numAnts0;
    int m_numAnts1;
    int m_numAnts2;
    int m_numAnts3;
    int m_numColonies;
    std::vector<Actor*> m_world[VIEW_WIDTH][VIEW_HEIGHT];
    Compiler* m_compilerForEntrant0;
    Compiler* m_compilerForEntrant1;
    Compiler* m_compilerForEntrant2;
    Compiler* m_compilerForEntrant3;
    int m_ticks;
};


#endif // STUDENTWORLD_H_
