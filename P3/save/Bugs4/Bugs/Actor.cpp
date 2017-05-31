#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <math.h>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//===Actor===
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, int depth)
: GraphObject(imageID, startX, startY, GraphObject::right, depth), m_sw(sw){
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

bool Actor::isObstacle() const{
    return false;
}

bool Actor::isFood() const{
    return false;
}

bool Actor::isPheromone() const{
    return false;
}

bool Actor::isAnt() const{
    return false;
}

bool Actor::isGHopper() const{
    return false;
}

bool Actor::isColony() const{
    return false;
}

bool Actor::isDead() const{
    return false;
}

bool Actor::isDanger() const{
    return false;
}

int Actor::getEnergy() const{
    return 0;
}

int Actor::getColony() const{
    return -1;
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
            y++;
            break;
        case GraphObject::right:
            x++;
            break;
        case GraphObject::down:
            y--;
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

bool Pebble::isObstacle() const{
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

bool Poison::isDanger() const{
    return true;
}

//===Energy===
Energy::Energy(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy)
: Actor(sw, imageID, startX, startY, depth), m_energy(energy){
}

int Energy::tryChangeEnergy(int diff){
    if (m_energy + diff < 0){
        int temp = (-1) * m_energy;
        m_energy = 0;
        return temp;
    }
    m_energy += diff;
    //std::cout << "diff: " << diff << std::endl;
    return diff;//Still has energy
}

bool Energy::tryEat(int diff){
    //std::cout << "tryEat: ";
    int energyEaten = getSW()->eatFood(getX(), getY(), diff);
    if (energyEaten == 0)
        return false;
    tryChangeEnergy(energyEaten);
    return true;
}

bool Energy::isDead() const{
    return !m_energy;
}

int Energy::getEnergy() const{
    return m_energy;
}

//===Food===
Food::Food(StudentWorld* sw, int startX, int startY, int energy)
: Energy(sw, IID_FOOD, startX, startY, 1, energy){
}

void Food::doSomething(){
    //std::cout << "energy: " << getEnergy() << std::endl;
}

bool Food::isFood() const{
    return true;
}

//===Pheromone===
Pheromone::Pheromone(StudentWorld* sw, int imageID, int startX, int startY, int colony)
: Energy(sw, imageID, startX, startY, 2, 256), m_colony(colony){
}

void Pheromone::doSomething(){
    tryChangeEnergy(-1);                                            //1 2. Decrease strength and die
}

bool Pheromone::isPheromone() const{
    return true;
}

int Pheromone::getColony() const{
    return m_colony;
}

//===AntHill===
AntHill::AntHill(StudentWorld* sw, int startX, int startY, int numColony, Compiler* comp)
: Energy(sw, IID_ANT_HILL, startX, startY, 2, 8999), m_numColony(numColony), m_comp(comp){
}
AntHill::~AntHill(){
//    delete m_comp;
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

bool AntHill::isColony() const{
    return true;
}

int AntHill::getColony() const{
    return m_numColony;
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
    //std::cout << getEnergy() << " ";
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

void Mover::addStun(){
    //std::cout << "addStun" << std::endl;
    if (m_stunned)
        return;
    
    m_sleepTime += 2;
    m_stunned = true;
}

void Mover::addSleep(){
    //std::cout << "addSleep" << std::endl;
    m_sleepTime = 3;
}

void Mover::decTime(){
    if (m_sleepTime == 0)
        m_stunned = false;
    if (m_sleepTime > 0)
        m_sleepTime--;

   // std::cout << "sleep: " << m_sleepTime << ", stun: " << m_stunTime << ", stunned: " << m_stunned << std::endl;
}

bool Mover::isInactive() const{
    if (m_sleepTime >= 1)
        return true;
    return false;
}

//===Ant===
Ant::Ant(StudentWorld* sw, int imageID, int startX, int startY, int numColony, Compiler* comp)
: Mover(sw, imageID, startX, startY, 1, 1500), m_numColony(numColony), m_comp(comp), m_food(0), m_counter(0), m_lastRandom(0), m_wasBitten(false), m_wasBlocked(false){
    setDirection((Direction) randInt(1,4));                         //Random dir
}

void Ant::doSomething(){
    tryChangeEnergy(-1) ;                                           //1. Hunger
    
    StudentWorld* sw = getSW();
    
    if (isDead()){                                                  //2. Check if dead
        sw->addFood(getX(), getY(), 100);                           //2a. Add food
        return;                                                     //2b. Implicitly die. 2c. Return
    }
    
    decTime();                                                      //3a. Dec stun
    if (isInactive())                                               //3. Check if stunned
        return;                                                     //3b. Return

    Compiler::Command cmd;
    
    for (int a = 0; a < 1; a++){                                   //4. Between 1 and 10 commands
        if (!m_comp->getCommand(m_counter, cmd)) {                  //4a. Next command
            tryChangeEnergy(-1 * getEnergy());                      //4bi. Set ant to dead
            return;                                                 //4bii. Return
        }
        int req = 400;
        int x, y;
        switch (cmd.opcode){                                        //4c. Execute next command
            case Compiler::moveForward:
                int nextX, nextY;
                nextPos(nextX, nextY);
                if (sw->hasObstacle(nextX, nextY) == false){
                    moveTo(nextX, nextY);
                    m_wasBlocked = false;
                    m_wasBitten = false;
                }
                else{
                    m_wasBlocked = true;
                }
                m_counter++;
                //std::cout << "moveFwd";
                return;
                break;
            case Compiler::eatFood:
                if (m_food >= 100){
                    m_food -= 100;
                    tryChangeEnergy(100);
                }
                else{
                    tryChangeEnergy(m_food);
                    m_food = 0;
                }
                m_counter++;
                return;
                break;
            case Compiler::dropFood:
                sw->addFood(getX(), getY(), m_food);
                m_food = 0;
                m_counter++;
                return;
                break;
            case Compiler::bite:
                sw->biteEnemy(getX(), getY(), getColony());
                m_counter++;
                return;
                break;
            case Compiler::pickupFood:
                if (m_food > 1400)
                    req = 1800 - m_food;
                m_food += sw->eatFood(getX(), getY(), req);
                //std::cout << m_food;
                m_counter++;
                return;
                break;
            case Compiler::emitPheromone:
                sw->addPheromone(getX(), getY(), getColony());
                m_counter++;
                return;
                break;
            case Compiler::faceRandomDirection:
                setDirection((Direction) randInt(1, 4));
                m_counter++;
                return;
                break;
            case Compiler::rotateClockwise:
                switch ((int) getDirection()){
                    case 1:
                        setDirection(Direction::right);
                        break;
                    case 2:
                        setDirection(Direction::down);
                        break;
                    case 3:
                        setDirection(Direction::left);
                        break;
                    case 4:
                        setDirection(Direction::up);
                        break;
                }
                m_counter++;
                return;
                break;
            case Compiler::rotateCounterClockwise:
                switch ((int) getDirection()){
                    case 1:
                        setDirection(Direction::left);
                        break;
                    case 2:
                        setDirection(Direction::up);
                        break;
                    case 3:
                        setDirection(Direction::right);
                        break;
                    case 4:
                        setDirection(Direction::down);
                        break;
                }
                m_counter++;
                return;
                break;
            case Compiler::generateRandomNumber:
                m_lastRandom = randInt(0, stoi(cmd.operand1));
                m_counter++;
                //std::cout << "genRand";
                break;
            case Compiler::goto_command:
                m_counter = stoi(cmd.operand1);
                //std::cout << "goto";
                break;
            case Compiler::if_command:
                switch(stoi(cmd.operand1)){
                    case Compiler::last_random_number_was_zero:
                        if (m_lastRandom == 0)
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_am_carrying_food:
                        if (m_food > 0)
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_am_hungry:
                        if (getEnergy() <= 25)
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_am_standing_with_an_enemy:
                        if (sw->hasEnemy(getX(), getY(), getColony()))
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_am_standing_on_food:
                        if (sw->hasFood(getX(), getY()))
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        //std::cout << "standingOnFood";
                        break;
                    case Compiler::i_am_standing_on_my_anthill:
                        if (sw->hasHill(getX(), getY(), getColony()))
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_smell_pheromone_in_front_of_me:
                        //std::cout << "x :" << getX() << ", y :" << getY() << ". ";
                        nextPos(x, y);
                        std::cout << "x :" << x << ", y :" << y;
                        if (sw->hasPheromone(x, y, getColony()))
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_smell_danger_in_front_of_me:
                        nextPos(x, y);
                        if (sw->hasDanger(x, y, getColony()))
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_was_bit:
                        if (m_wasBitten)
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                    case Compiler::i_was_blocked_from_moving:
                        if (m_wasBlocked)
                            m_counter = stoi(cmd.operand2);
                        else
                            m_counter++;
                        break;
                }
                break;
        }
    }
}

bool Ant::isAnt() const{
    return true;
}

int Ant::getColony() const{
    return m_numColony;
}

//===Grasshopper===
Grasshopper::Grasshopper(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy)
: Mover(sw, imageID, startX, startY, depth, energy){
    changeDirSteps();
}

bool Grasshopper::isGHopper() const{
    return true;
}

bool Grasshopper::isDanger() const{
    return true;
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
        addSleep();                                                 //7. 1/2 chance Step 12
        return;
    }
    
    if (getSteps() == 0)                                            //8. Check steps
        changeDirSteps();                                           //8a 8b. New dir/steps
    
    int nextX, nextY;
    nextPos(nextX, nextY);                                          //9. Attempt steps
    if (sw->hasObstacle(nextX, nextY) == false){
        moveTo(nextX, nextY);
        decSteps();                                                 //11. Dec step count
    }
    else
        changeDirSteps();                                           //10a 10b. New dir/steps Step 12
    
    addSleep();                                                      //12. Sleep
    
    //std::cout << "energy: " << getEnergy() << std::endl;
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
        addSleep();                                                 //5b. Step 13
        return;
    }
    else if (randInt(1,10) == 1){                                   //6. 1/10 chance to jump
        int angle = randInt(0,359);
        int cosPos = 10 * cos(angle * 3.14159265 / 180.0) + getX();
        int sinPos = 10 * sin(angle * 3.14159265 / 180.0) + getY();
        if ((cosPos > 0 && cosPos < VIEW_WIDTH)                     //6a. Circular pos
            && (sinPos > 0 && sinPos < VIEW_HEIGHT)
            && sw->hasObstacle(cosPos, sinPos) == false){
            moveTo(cosPos, sinPos);
            addSleep();                                              //6b. Step 13
            return;
        }
    }
    
    if (tryEat(200) && randInt(1,2) == 1){                          //7. Eat
        addSleep();                                                 //8. 1/2 chance Step 13
        return;
    }
    
    if (getSteps() == 0)                                            //9. Check steps
        changeDirSteps();                                           //9a 9b. New dir/steps
    
    int nextX, nextY;
    nextPos(nextX, nextY);                                          //10. Attempt steps
    if (sw->hasObstacle(nextX, nextY) == false){
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









