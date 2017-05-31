#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Compiler;

//=== === === === === Actor -> GraphObject
class Actor : public GraphObject{
public:
    Actor(StudentWorld* sw, int imageID, int startX, int startY, int depth);
    virtual void doSomething() = 0;
    
    virtual bool isAnObstacle() const;//returns false, overriden by Pebble
    virtual bool isAFood() const;//returns false, overriden by Food
    virtual bool isDead() const;//returns false, overriden by Energy
    virtual int tryChangeEnergy(int diff);//if has energy, will change energy
    virtual bool tryGetPoisoned();//if poisonable, will get poisoned
    virtual bool tryGetBitten(int damage);//if bitable, will get biten
    virtual bool tryGetStunned();//if stunnable, will get stunned
    
protected:
    void currentPos(int& x, int& y) const;
    void nextPos(int& x, int& y) const;
    StudentWorld* getSW() const;
    
private:
    StudentWorld* m_sw;
};

//=== Pebble -> Actor -> GraphObject
class Pebble : public Actor{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool isAnObstacle() const;//returns true
};

//===Water -> Actor -> GraphObject
class Water : public Actor{
public:
    Water(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
private:
};

//=== Poison -> Actor -> GraphObject
class Poison : public Actor{
public:
    Poison(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
};

//=== === === === Energy  -> Actor -> GraphObject
class Energy : public Actor{
public:
    Energy(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy);
    
    virtual bool isDead() const;
    virtual int tryChangeEnergy(int diff);//if has energy, will change energy
    bool tryEat(int diff);
    int getEnergy() const;
    
private:
    int m_energy;
};

//===Food -> Energy  -> Actor -> GraphObject
class Food : public Energy{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isAFood() const;
private:
};

//===Pheromone -> Energy  -> Actor -> GraphObject
class Pheromone : public Energy{
public:
    Pheromone(StudentWorld* sw, int imageID, int startX, int startY);
    virtual void doSomething();
private:
};

//===AntHill -> Energy  -> Actor -> GraphObject
class AntHill : public Energy{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int numColony, Compiler* comp);
    virtual void doSomething();
private:
    int m_numColony;
    Compiler* m_comp;
};

//=== === === Mover -> Energy  -> Actor -> GraphObject
class Mover : public Energy{
public:
    Mover(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy);
    
    virtual bool tryGetPoisoned();//if poisonable, will get poisoned
    virtual bool tryGetBitten(int damage);//if bitable, will get biten
    virtual bool tryGetStunned();//if stunnable, will get stunned
    bool tryBite(int damage);//if can bite, bites
    
    bool isInactive() const;
    void addStun();
    void addSleep();
    void decTime();
    
private:
    bool m_stunned;
    int m_stunTime;
    int m_sleepTime;
};

//===Ant -> Mover -> Energy  -> Actor -> GraphObject
class Ant : public Mover{
public:
    Ant(StudentWorld* sw, int imageID, int startX, int startY, int numColony, Compiler* comp);
    virtual void doSomething();
    int getColony() const;
    
private:
    void doCommand();
    
    int m_numColony;
    int m_food;
    int m_counter;
    int m_lastRandom;
    bool m_wasBitten;
    bool m_wasBlocked;
    Compiler* m_comp;
};

//=== === Grasshopper -> Mover -> Energy  -> Actor -> GraphObject
class Grasshopper : public Mover{
public:
    Grasshopper(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy);
    virtual void doSomething() = 0;
protected:
    int getSteps() const;
    void decSteps();
    void changeDirSteps();
private:
    int m_steps;
};

//===BabyGrasshopper -> Grasshopper -> Mover -> Energy  -> Actor -> GraphObject
class BabyGrasshopper : public Grasshopper{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
private:
};

//===AdultGrasshopper -> Grasshopper -> Mover -> Energy  -> Actor -> GraphObject
class AdultGrasshopper : public Grasshopper{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool tryGetPoisoned();
    virtual bool tryGetBitten(int damage);
    virtual bool tryGetStunned();
private:
};



























#endif // ACTOR_H_
