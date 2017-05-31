#include "StudentWorld.h"
#include "Compiler.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_ticks(0), m_numAnts0(0), m_numAnts1(0), m_numAnts2(0), m_numAnts3(0){
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init(){
    Field f;
    if (f.loadField(getFieldFilename()) != 0)
        return GWSTATUS_LEVEL_ERROR;
    
    Compiler *compilerForEntrant0, *compilerForEntrant1, *compilerForEntrant2, *compilerForEntrant3;
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    std::string error;

    //empty, anthill0, anthill1, anthill2, anthill3, food, grasshopper, water, rock, poison
    
    for (int x = 0; x < VIEW_WIDTH; x++){
        for (int y = 0; y < VIEW_HEIGHT; y++){
            switch (f.getContentsOf(x, y)){
                case Field::empty:
                    break;
                case Field::anthill0:
                    compilerForEntrant0 = new Compiler;
                    if (!compilerForEntrant0->compile(fileNames[0], error))
                    {
                        setError(fileNames[0] + " " + error);
                        return GWSTATUS_LEVEL_ERROR;
                    }
                    m_world[x][y].push_back(new AntHill(this, x, y, 0, compilerForEntrant0));
                    break;
                case Field::anthill1:
                    compilerForEntrant1 = new Compiler;
                    if (!compilerForEntrant1->compile(fileNames[1], error))
                    {
                        setError(fileNames[1] + " " + error);
                        return GWSTATUS_LEVEL_ERROR;
                    }
                    m_world[x][y].push_back(new AntHill(this, x, y, 1, compilerForEntrant1));
                    break;
                case Field::anthill2:
                    compilerForEntrant2 = new Compiler;
                    if (!compilerForEntrant2->compile(fileNames[2], error))
                    {
                        setError(fileNames[2] + " " + error);
                        return GWSTATUS_LEVEL_ERROR;
                    }
                    m_world[x][y].push_back(new AntHill(this, x, y, 2, compilerForEntrant2));
                    break;
                case Field::anthill3:
                    compilerForEntrant3 = new Compiler;
                    if (!compilerForEntrant3->compile(fileNames[3], error))
                    {
                        setError(fileNames[3] + " " + error);
                        return GWSTATUS_LEVEL_ERROR;
                    }
                    m_world[x][y].push_back(new AntHill(this, x, y, 3, compilerForEntrant3));
                    break;
                case Field::food:
                    m_world[x][y].push_back(new Food(this, x, y, 6000));
                    break;
                case Field::grasshopper:
                    m_world[x][y].push_back(new BabyGrasshopper(this, x, y));
                    break;
                case Field::water:
                    m_world[x][y].push_back(new Water(this, x, y));
                    break;
                case Field::rock:
                    m_world[x][y].push_back(new Pebble(this, x, y));
                    break;
                case Field::poison:
                    m_world[x][y].push_back(new Poison(this, x, y));
                    break;
            }
        }
    }
    
    //addAdult(19, 49);
    //addAdult(19, 49);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    m_ticks++;
    
    std::cout << "===m_ticks: " << m_ticks << "===" << std::endl;
    
    //Enumerate actors into list
    std::vector<Actor*> list;
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
            for (int z = 0; z < m_world[x][y].size(); z++)
                list.push_back(m_world[x][y].at(z));
    
    
    //Call doSomething for each actor
    for (auto it = list.begin(); it < list.end(); it++){
        int prevX = (*it)->getX();
        int prevY = (*it)->getY();
        
        (*it)->doSomething();
        
        int newX = (*it)->getX();
        int newY = (*it)->getY();
        
        if (prevX != newX || prevY != newY){
            eraseActor(prevX, prevY, (*it));
            m_world[newX][newY].push_back((*it));
        }
    }
    
    //Remove each dead actor
    for (auto it = list.begin(); it < list.end(); it++){
        if ((*it)->isDead()){
            eraseActor((*it)->getX(), (*it)->getY(), (*it));
            delete (*it);
        }
    }
    
    //update status text
    string output = "Ticks:";
    ostringstream oss;
    oss << setw(5) << m_ticks << endl;
    output += oss.str();
    
    setGameStatText(output);
    
    //end game
    if (isGameOver() || m_ticks >= 2000){
        if (getWinnerName() == "No winner!")
            return GWSTATUS_NO_WINNER;
        else{
            //setWinner();
            return GWSTATUS_PLAYER_WON;
        }
    }
    std::cout << std::endl << std::endl;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
            for (auto it = m_world[x][y].begin(); it < m_world[x][y].end(); it++){
                m_world[x][y].erase(it);
                delete *it;
            }
}

bool StudentWorld::obstacleAtPos(int x, int y){
    if (x <= 0 || y <= 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return true;
    
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isAnObstacle())
            return true;
    return false;
}

int StudentWorld::eatFood(int x, int y, int energy){
    if (x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return 0;
    
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isAFood())
            return (-1) * (*it)->tryChangeEnergy((-1) * energy);
    return 0;
}

void StudentWorld::addFood(int x, int y, int energy){
    bool hasFoodAlready = false;
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if ((*it)->isAFood()){
            hasFoodAlready = true;
            (*it)->tryChangeEnergy(energy);                                 //only one food object per square
            break;
        }
    }
    
    if (hasFoodAlready == false)
        m_world[x][y].push_back(new Food(this, x, y, energy));
}

void StudentWorld::addAdult(int x, int y){
    m_world[x][y].push_back(new AdultGrasshopper(this, x, y));
}

void StudentWorld::addAnt(int x, int y, int colony, Compiler* comp){
    switch (colony){
        case 0:
            m_world[x][y].push_back(new Ant(this, IID_ANT_TYPE0, x, y, colony, comp));
            m_numAnts0++;
            break;
        case 1:
            m_world[x][y].push_back(new Ant(this, IID_ANT_TYPE1, x, y, colony, comp));
            m_numAnts1++;
            break;
        case 2:
            m_world[x][y].push_back(new Ant(this, IID_ANT_TYPE2, x, y, colony, comp));
            m_numAnts2++;
            break;
        case 3:
            m_world[x][y].push_back(new Ant(this, IID_ANT_TYPE3, x, y, colony, comp));
            m_numAnts3++;
            break;
    }
}

void StudentWorld::poisonAll(int x, int y){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        (*it)->tryGetPoisoned();
}

void StudentWorld::stunAll(int x, int y){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        (*it)->tryGetStunned();
}

void StudentWorld::biteOne(int x, int y, Actor* me, int damage){
    //std::cout << "x: " << x << ", y: " << y << ", me: " << me << ", damage: " << damage << std::endl;
    //std::cout << "biteOne " << std::endl;
    std::vector<Actor*> list;
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if (*it != me && (*it)->tryGetBitten(0)){
            list.push_back(*it);
            //std::cout << "push_back ";
            break;
        }
    }
    
    //std::cout << "list.size(): " << list.size() << std::endl;
    if (list.size() != 0){
        auto it = list.begin();
        //std::cout << "bitten : " << *it << std::endl;
        ((*it) + randInt(0, (int) list.size() - 1))->tryGetBitten(damage);
    }
}

void StudentWorld::eraseActor(int x, int y, Actor* currActor){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if (currActor == *it){
            m_world[x][y].erase(it);
            break;
        }
    }
}






















