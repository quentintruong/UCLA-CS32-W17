#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//===Actor===
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, int depth)
: GraphObject(imageID, startX, startY, GraphObject::right, depth), m_sw(sw){
}

bool Actor::isAnObstacle() const{
    return false;
}

bool Actor::isAFood() const{
    return false;
}

bool Actor::isDead() const{
    return false;
}

int Actor::tryChangeEnergy(int diff){
    return 0;
}

bool Actor::tryGetPoisoned(){
    return false;
}

bool Actor::tryGetBitten(int damage){
    return false;
}

bool Actor::tryGetStunned(){
    return false;
}

void Actor::currentPos(int& x, int& y) const{
    x = getX();
    y = getY();
}

void Actor::nextPos(int& x, int& y) const{
    currentPos(x, y);
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
}

StudentWorld* Actor::getSW() const{
    return m_sw;
}

//===Pebble===
Pebble::Pebble(StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_ROCK, startX, startY, 1){
}

void Pebble::doSomething(){
}

bool Pebble::isAnObstacle() const{
    return true;
}

//===Water===
Water::Water(StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_WATER_POOL, startX, startY, 2){
}

void Water::doSomething(){
    getSW()->stunAll(getX(), getY());
}

//===Poison===
Poison::Poison(StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_POISON, startX, startY, 2){
}

void Poison::doSomething(){
    getSW()->poisonAll(getX(), getY());                             //1. Poison all on same square
}

//===Energy===
Energy::Energy(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy)
: Actor(sw, imageID, startX, startY, depth), m_energy(energy){
}

bool Energy::isDead() const{
    return !m_energy;
}

int Energy::tryChangeEnergy(int diff){
    if (m_energy + diff < 0){
        int temp = (-1) * m_energy;
        m_energy = 0;
        return temp;
    }
    m_energy += diff;
    //std::cout << "tryChangeEnergy this: " << this << ", diff: " << diff << ", energy: " << m_energy << std::endl;
    return diff;//Still has energy
}

bool Energy::tryEat(int diff){
    //std::cout << "tryEat: ";
    StudentWorld* sw = getSW();
    int energyEaten = sw->eatFood(getX(), getY(), diff);
    tryChangeEnergy(energyEaten);
    if (energyEaten == 0)
        return false;
    return true;
}

int Energy::getEnergy() const{
    return m_energy;
}

//===Food===
Food::Food(StudentWorld* sw, int startX, int startY, int energy)
: Energy(sw, IID_FOOD, startX, startY, 1, energy){
}

void Food::doSomething(){//done
}

bool Food::isAFood() const{
    return true;
}

//===Pheromone===
Pheromone::Pheromone(StudentWorld* sw, int imageID, int startX, int startY)
: Energy(sw, imageID, startX, startY, 2, 256){
    
}

void Pheromone::doSomething(){
    tryChangeEnergy(-1);                                            //1 2. Decrease strength and die
}

//===AntHill===
AntHill::AntHill(StudentWorld* sw, int startX, int startY, int numColony, Compiler* comp)
: Energy(sw, IID_ANT_HILL, startX, startY, 2, 8999), m_numColony(numColony), m_comp(comp){
}

void AntHill::doSomething(){
    tryChangeEnergy(-1);;                                           //1. Hunger
    
    StudentWorld* sw = getSW();
    
    if (isDead())                                                   //2. Check if dead
        return;                                                     //2a 2b. Implicitly die and return
    
    if (tryEat(10000))                                              //3a. Eat
        return;                                                     //3b. Return
    
    if (getEnergy() >= 2000){                                       //4. Produce new ant
        sw->addAnt(getX(), getY(), m_numColony, m_comp);            //4a 4c. Add ant
        tryChangeEnergy(-1500);                                     //4b. Reduce by 1500
    }
}

//=== === Mover === ===
Mover::Mover(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy)
: Energy(sw, imageID, startX, startY, depth, energy), m_sleepTime(0), m_stunTime(0), m_stunned(false){
    
}

bool Mover::tryGetPoisoned(){
    //std::cout << "tryGetPoisoned: ";
    tryChangeEnergy(-150);
    return true;
}

bool Mover::tryGetBitten(int damage){
    tryChangeEnergy(-1 * damage);
    return true;
}

bool Mover::tryGetStunned(){
    //std::cout << "tryGetStunned: ";
    if (m_stunned)
        return false;
    addStun();
    return true;
}

bool Mover::tryBite(int damage){
    getSW()->biteOne(getX(), getY(), this, damage);
    return true;
}

bool Mover::isInactive() const{
    if (m_sleepTime >= 0)
        return true;
    return false;
}

void Mover::addStun(){
    //std::cout << "addStun" << std::endl;
    m_stunTime = 2;
    m_sleepTime += 2;
    m_stunned = true;
}

void Mover::addSleep(){
    //std::cout << "addSleep" << std::endl;
    if (m_sleepTime == -1)
        m_sleepTime = 0;
    m_sleepTime += 2;
}

void Mover::decTime(){
    if (m_sleepTime >= 0)
        m_sleepTime--;
    m_stunTime--;
    
    if (m_stunTime < 0 && m_sleepTime < 0)
        m_stunned = false;
    //std::cout << false;
    //std::cout << "sleep: " << m_sleepTime << ", stun: " << m_stunTime << ", stunned: " << m_stunned << std::endl;
}


//===Ant===
Ant::Ant(StudentWorld* sw, int imageID, int startX, int startY, int numColony, Compiler* comp)
: Mover(sw, imageID, startX, startY, 1, 1500), m_numColony(numColony), m_comp(comp), m_food(0), m_counter(0), m_lastRandom(0), m_wasBitten(false), m_wasBlocked(false){
    setDirection((Direction) randInt(1,4));                         //Random dir
}

void Ant::doSomething(){
    tryChangeEnergy(-1);;                                           //1. Hunger
    
    StudentWorld* sw = getSW();
    
    if (isDead()){                                                  //2. Check if dead
        sw->addFood(getX(), getY(), 100);                           //2a. Add food
        return;                                                     //2b. Implicitly die. 2c. Return
    }
    
    decTime();                                                      //3a. Dec stun
    if (isInactive()){                                              //3. Check if stunned
        return;                                                     //3b. Return
    }
}

int Ant::getColony() const{
    return m_numColony;
}

void Ant::doCommand(){
    
}

//===Grasshopper===
Grasshopper::Grasshopper(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy)
: Mover(sw, imageID, startX, startY, depth, energy){
    changeDirSteps();
}

int Grasshopper::getSteps() const{
    return m_steps;
}

void Grasshopper::decSteps(){
    m_steps--;
}

void Grasshopper::changeDirSteps(){
    setDirection((Direction) randInt(1,4));                         //Random dir
    //setDirection(Direction :: right);
    m_steps = randInt(2,10);                                        //Random dist [2,10] inclusive
}

//===BabyGrasshopper===
BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, IID_BABY_GRASSHOPPER, startX, startY, 1, 500){
}

void BabyGrasshopper::doSomething(){
    tryChangeEnergy(-1);;                                           //1. Hunger
    
    StudentWorld* sw = getSW();
    
    if (isDead()){                                                  //2. Check if dead
        sw->addFood(getX(), getY(), 100);                           //2a. Add food
        return;                                                     //2b. Implicitly die. 2c. Return
    }
    
    decTime();                                                      //3a. Dec sleep
    if (isInactive()){                                              //3. Check asleep
        return;                                                     //3b. Return
    }
    
    if (getEnergy() >= 1600){                                       //5. Check energy
        sw->addAdult(getX(), getY());                               //5a. Add adult
        tryChangeEnergy((-1) * getEnergy());                        //5b. Die
        sw->addFood(getX(), getY(), 100);                           //5b. Add food
        return;                                                     //5c. Return
    }
    
    if (tryEat(200) && randInt(1,2) == 1){                          //6. Eat
        addSleep();                                                  //7. 1/2 chance Step 12
        return;
    }
    
    if (getSteps() == 0)                                            //8. Check steps
        changeDirSteps();                                           //8a 8b. New dir/steps
    
    int nextX, nextY;
    nextPos(nextX, nextY);                                          //9. Attempt steps
    if (sw->obstacleAtPos(nextX, nextY) == false){
        moveTo(nextX, nextY);
        decSteps();                                                 //11. Dec step count
    }
    else
        changeDirSteps();                                           //10a 10b. New dir/steps Step 12
    
    addSleep();                                                      //12. Sleep
}

//===AdultGrasshopper===
AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, IID_ADULT_GRASSHOPPER, startX, startY, 1, 1600){
}

void AdultGrasshopper::doSomething(){
    tryChangeEnergy(-1);                                            //1. Hunger
    
    StudentWorld* sw = getSW();
    
    if (isDead()){                                                  //2. Check if dead
        sw->addFood(getX(), getY(), 100);                           //2a. Add food
        return;                                                     //2b. Implicitly die. 2c. Return
    }
    
    decTime();                                                      //3a. Dec sleep
    if (isInactive()){                                              //3. Check asleep
        return;                                                     //3b. Return
    }
    
    if (randInt(1,3) == 1){                                         //5. 1/3 chance to bite
        //std::cout << "tryBite; biter: " << this << std::endl;
        tryBite(50);                                                //5a. Bite
        addSleep();                                                  //5b. Step 13
        return;
    }
    else if (randInt(1,10) == 1){                                   //6. 1/10 chance to jump
        int angle = randInt(0,359);
        int cosPos = 10 * cos(angle * 3.14159265 / 180.0) + getX();
        int sinPos = 10 * sin(angle * 3.14159265 / 180.0) + getY();
        if ((cosPos > 0 && cosPos < VIEW_WIDTH)                     //6a. Circular pos
            && (sinPos > 0 && sinPos < VIEW_HEIGHT)
            && sw->obstacleAtPos(cosPos, sinPos) == false){
            moveTo(cosPos, sinPos);
            addSleep();                                              //6b. Step 13
            return;
        }
    }
    
    if (tryEat(200) && randInt(1,2) == 1){                          //7. Eat
        addSleep();                                                  //8. 1/2 chance Step 13
        return;
    }
    
    if (getSteps() == 0)                                            //9. Check steps
        changeDirSteps();                                           //9a 9b. New dir/steps
    
    int nextX, nextY;
    nextPos(nextX, nextY);                                          //10. Attempt steps
    if (sw->obstacleAtPos(nextX, nextY) == false){
        moveTo(nextX, nextY);
        decSteps();                                                 //12. Dec step count
    }
    else
        changeDirSteps();                                           //11a 11b. New dir/steps
    
    addSleep();                                                      //13. Sleep
}

bool AdultGrasshopper::tryGetPoisoned(){                            //No effect on adult
    return false;
}

bool AdultGrasshopper::tryGetBitten(int damage){                    //Retailate
    tryChangeEnergy(-1 * damage);
    if (isDead() == false)                                          //If not dead, 1/2 chance to bite
        if (randInt(1,2) == 1)
            tryBite(50);
    return true;
}

bool AdultGrasshopper::tryGetStunned(){                             //No effect on adult
    return false;
}









