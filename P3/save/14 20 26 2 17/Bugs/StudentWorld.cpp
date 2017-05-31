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

    //addAnt(49, 9, 1, compilerForEntrant1);
    //addAnt(20, 21, 0, compilerForEntrant0);
    
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
    for (auto it = list.begin(); it != list.end(); it++){
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
    for (auto it = list.begin(); it != list.end(); it++){
        if ((*it)->isDead()){
            eraseActor((*it)->getX(), (*it)->getY(), (*it));
            delete (*it);
        }
    }
    
    //Update status text
    ostringstream oss;
    oss << setw(5) << m_ticks;
    string output = "Ticks:" + oss.str() + " - ";
    
    int winningAntNumber = getWinningAntColony();
    
    //std::vector<std::string> names = getFilenamesOfAntPrograms();
    std::vector<std::string> names;
    names.push_back("a");
    names.push_back("b");
    names.push_back("c");
    names.push_back("d");
    for (int a = 0; a < 4; a++){
        ostringstream oss2;
        oss2 << setw(2) << setfill('0') << getNumberOfAntsFor(a);
        output += names[a];
        if (winningAntNumber == a)
            output += "*";
        output += ": " + oss2.str() + " ";
    }
    setGameStatText(output);
    
    //end game
    if (isGameOver() || m_ticks >= 2000){
        if (weHaveAWinningAnt()){
            setWinner(names[winningAntNumber]);
            return GWSTATUS_PLAYER_WON;
        }
        else{
            return GWSTATUS_NO_WINNER;
        }
    }
    std::cout << std::endl << std::endl;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
            for (int z = 0; z < m_world[x][y].size(); z++){
                std::cout << x << " " << y << " " << z << std::endl;
                delete m_world[x][y][z];
                m_world[x][y].erase(m_world[x][y].begin() + z);
                
            }
}

int StudentWorld::eatFood(int x, int y, int energy){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isFood())
            return (-1) * (*it)->tryChangeEnergy((-1) * energy);
    return 0;
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
    std::vector<Actor*> list;
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if (*it != me && ((*it)->isAnt() || (*it)->isGHopper()))
            list.push_back(*it);
    
    if (list.size() != 0){
        //std::cout << ", size: " << list.size() << ", me: " << me << ", *it: " << *list.begin() << ", *it+1: " << *(list.begin() + 1) << std::endl;
        //std::cout << *(it + randInt(0, list.size() - 1)) << std::endl;
        (*(list.begin() + randInt(0, (int) list.size() - 1)))->tryGetBitten(damage);
    }
}

void StudentWorld::biteEnemy(int x, int y, int colony){
    //std::cout << "antbite this: " << this << std::endl;
    std::vector<Actor*> list;
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isGHopper() || ((*it)->isAnt() && (*it)->getColony() != colony))
            list.push_back(*it);
    
    if (list.size() != 0){
        //std::cout << ", size: " << list.size() << ", me: " << me << ", *it: " << *list.begin() << ", *it+1: " << *(list.begin() + 1) << std::endl;
        //std::cout << *(it + randInt(0, list.size() - 1)) << std::endl;
        (*(list.begin() + randInt(0, (int) list.size() - 1)))->tryGetBitten(15);
    }
}

void StudentWorld::addFood(int x, int y, int energy){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if ((*it)->isFood()){
            (*it)->tryChangeEnergy(energy);                                 //only one food object per square
            return;
        }
    }
    
    m_world[x][y].push_back(new Food(this, x, y, energy));
}

void StudentWorld::addPheromone(int x, int y, int colony){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if ((*it)->isPheromone()){
            int amt = 256;
            int currEnergy = (*it)->getEnergy();
            if (768 - currEnergy < 256)
                amt = 768 - currEnergy;
            (*it)->tryChangeEnergy(amt);
            return;
        }
    }
    
    switch (colony){
        case 0:
            m_world[x][y].push_back(new Pheromone(this, IID_PHEROMONE_TYPE0, x, y, colony));
            break;
        case 1:
            m_world[x][y].push_back(new Pheromone(this, IID_PHEROMONE_TYPE1, x, y, colony));
            break;
        case 2:
            m_world[x][y].push_back(new Pheromone(this, IID_PHEROMONE_TYPE2, x, y, colony));
            break;
        case 3:
            m_world[x][y].push_back(new Pheromone(this, IID_PHEROMONE_TYPE3, x, y, colony));
            break;
    }
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

void StudentWorld::addAdult(int x, int y){
    m_world[x][y].push_back(new AdultGrasshopper(this, x, y));
}

bool StudentWorld::hasObstacle(int x, int y){
    if (x <= 0 || y <= 0 || x >= VIEW_WIDTH - 1 || y >= VIEW_HEIGHT - 1)
        return true;
    
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isObstacle())
            return true;
    return false;
}

bool StudentWorld::hasFood(int x, int y){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isFood())
            return true;
    return false;
}

bool StudentWorld::hasPheromone(int x, int y){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isPheromone())
            return true;
    return false;
}

bool StudentWorld::hasEnemy(int x, int y, int colony){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isGHopper() || ((*it)->isAnt() && (*it)->getColony() != colony))
            return true;
    return false;
}

bool StudentWorld::hasHill(int x, int y, int colony){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isColony() && (*it)->getColony() == colony)
            return true;
    return false;
}

bool StudentWorld::hasDanger(int x, int y, int colony){
    for (auto it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it)
        if ((*it)->isDanger() || ((*it)->getColony() != colony))
            return true;
    return false;
}

void StudentWorld::eraseActor(int x, int y, Actor* currActor){
    for (int z = 0; z < m_world[x][y].size(); z++){
        if (m_world[x][y][z] == currActor){
            //std::cout << x << " " << y << " " << z << std::endl;
            m_world[x][y].erase(m_world[x][y].begin() + z);
        }
    }
}

int StudentWorld::getNumberOfAntsFor(int colony){
    switch (colony){
        case 0:
            return m_numAnts0;
        case 1:
            return m_numAnts1;
        case 2:
            return m_numAnts2;
        case 3:
            return m_numAnts3;
    }
    return -1;
}

int StudentWorld::getWinningAntColony(){
    int myAntNums[] = {m_numAnts0, m_numAnts1, m_numAnts2, m_numAnts3};
    int pos = 0;
    for (int a = 1; a < 4; a++){
        if (myAntNums[pos] < myAntNums[a])
            pos = a;
    }
    return pos;
}

bool StudentWorld::weHaveAWinningAnt(){
    int myAntNums[] = {m_numAnts0, m_numAnts1, m_numAnts2, m_numAnts3};
    int winningNum = getWinningAntColony();
    myAntNums[winningNum] = -1;
    if (myAntNums[winningNum] != m_numAnts0 &&
        myAntNums[winningNum] != m_numAnts1 &&
        myAntNums[winningNum] != m_numAnts2 &&
        myAntNums[winningNum] != m_numAnts3)
        return true;
    return false;
}
















