#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, int depth, bool isObstacle)
: GraphObject(imageID, startX, startY, dir, depth), m_sw(sw), m_isObstacle(isObstacle){
}

bool Actor::isDead() const{
    return false;
}

std::vector<int> Actor::currentPos() const{
    std::vector<int> coord;
    coord.push_back(getX());
    coord.push_back(getY());
    return coord;
}

std::vector<int> Actor::nextPos() const{
    int x = getX();
    int y = getY();
    switch(getDirection()){
        case GraphObject::none:
            break;
        case GraphObject::up:
            y--;
            break;
        case GraphObject::right:
            x++;
            break;
        case GraphObject::down:
            y++;
            break;
        case GraphObject::left:
            x--;
            break;
    }
    std::vector<int> coord;
    coord.push_back(x);
    coord.push_back(y);
    return coord;
}

StudentWorld* Actor::getSW() const{
    return m_sw;
}

bool Actor::isObstacle() const{
    return m_isObstacle;
}

//Animate
Animate::Animate(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, int depth, int energy, bool isObstacle)
: Actor(sw, imageID, startX, startY, dir, depth, isObstacle), m_energy(energy){
    isAsleep = false;
    isStunned = false;
}

bool Animate::isDead() const{
    return !m_energy;
}

void Animate::changeEnergy(int diff){
    m_energy += diff;
}

//Pebble
Pebble::Pebble(StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_ROCK, startX, startY, GraphObject::right, 1, true){
}

void Pebble::doSomething(){
}

//BabyGrasshopper
BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
: Animate(sw, IID_BABY_GRASSHOPPER, startX, startY, GraphObject::right, 1, 500, false){
    changeDirSteps();
}

void BabyGrasshopper::changeDirSteps(){
    setDirection((Direction) randInt(1,4));
    steps = randInt(2,10);
}

void BabyGrasshopper::doSomething(){
    //changeEnergy(-1);
    if (steps == 0){
        changeDirSteps();
    }
    
    std::vector<int> coord = nextPos();
    std::vector<Actor*> actors = getSW()->getActorsList(coord.at(0), coord.at(1));
    
    bool blocked = false;
    for (int currActor = 0; currActor < actors.size(); currActor++){
        if (actors.at(currActor)->isObstacle()){
            changeDirSteps();
            blocked = true;
        }
    }
    
    if (blocked == false){
        moveTo(coord.at(0), coord.at(1));
        steps--;
    }
    
}
















