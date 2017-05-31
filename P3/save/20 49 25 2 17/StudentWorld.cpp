#include "StudentWorld.h"
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
: GameWorld(assetDir), m_ticks(0){
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init(){
    Field f;
    if (f.loadField(getFieldFilename()) != 0)
        return GWSTATUS_LEVEL_ERROR;
    
    Actor* curr;
    for (int x = 0; x < VIEW_WIDTH; x++){
        for (int y = 0; y < VIEW_HEIGHT; y++){
            switch (f.getContentsOf(x, y)){
                case Field::grasshopper:
                    curr = new BabyGrasshopper(this, x, y);
                    m_world[x][y].push_back(curr);
                break;
                case Field::rock:
                    curr = new Pebble(this, x, y);
                    m_world[x][y].push_back(curr);
                break;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    m_ticks++;
    
    //enumerate
    std::vector<Actor*> list;
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
            for (int z = 0; z < m_world[x][y].size(); z++)
                list.push_back(m_world[x][y].at(z));
    
    int newX = 0;
    int newY = 0;
    
    //doSomething for each actors
    for (int currActor = 0; currActor < list.size(); currActor++){
        int prevX = list[currActor]->getX();
        int prevY = list[currActor]->getY();
        
        list[currActor]->doSomething();
        
        newX = list[currActor]->getX();
        newY = list[currActor]->getY();
        
        if (prevX != newX || prevY != newY){
            removeActorFromGrid(prevX, prevY, list.at(currActor));
            m_world[newX][newY].push_back(list[currActor]);
        }
    }
    
    //remove each dead actor
    for (int currActor = 0; currActor < list.size(); currActor++){
        if (list.at(currActor)->isDead()){
            removeActorFromGrid(newX, newY, list.at(currActor));
            delete list.at(currActor);
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
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
            for (int z = 0; z < m_world[x][y].size(); z++)
                delete m_world[x][y].at(z);
}

std::vector<Actor*> StudentWorld::getActorsList(int x, int y){
    return m_world[x][y];
}

void StudentWorld::removeActorFromGrid(int x, int y, Actor* currActor){
    for (std::vector<Actor*>::iterator it = m_world[x][y].begin(); it != m_world[x][y].end(); ++it){
        if (currActor == *it){
            m_world[x][y].erase(it);
            break;
        }
    }
}






















