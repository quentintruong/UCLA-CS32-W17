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
    
    virtual int tryChangeEnergy(int diff);//if has energy, will change energy
    virtual bool tryGetPoisoned();//if poisonable, will get poisoned
    virtual bool tryGetBitten(int damage);//if bitable, will get biten
    virtual bool tryGetStunned();//if stunnable, will get stunned
    
    virtual bool isObstacle() const;
    virtual bool isFood() const;
    virtual bool isPheromone() const;
    virtual bool isAnt() const;
    virtual bool isGHopper() const;
    virtual bool isColony() const;
    virtual bool isDead() const;
    virtual bool isDanger() const;
    
    virtual int getEnergy() const;
    virtual int getColony() const;
    
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
    
    virtual bool isObstacle() const;//returns true
};

//===Water -> Actor -> GraphObject
class Water : public Actor{
public:
    Water(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
};

//=== Poison -> Actor -> GraphObject
class Poison : public Actor{
public:
    Poison(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    
    virtual bool isDanger() const;
};

//=== === === === Energy  -> Actor -> GraphObject
class Energy : public Actor{
public:
    Energy(StudentWorld* sw, int imageID, int startX, int startY, int depth, int energy);
    
    virtual int tryChangeEnergy(int diff);//if has energy, will change energy
    bool tryEat(int diff);
    
    virtual bool isDead() const;
    
    virtual int getEnergy() const;
    
private:
    int m_energy;
};

//===Food -> Energy  -> Actor -> GraphObject
class Food : public Energy{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    
    virtual bool isFood() const;
};

//===Pheromone -> Energy  -> Actor -> GraphObject
class Pheromone : public Energy{
public:
    Pheromone(StudentWorld* sw, int imageID, int startX, int startY, int colony);
    virtual void doSomething();
    
    virtual bool isPheromone() const;
    
private:
    int m_colony;
};

//===AntHill -> Energy  -> Actor -> GraphObject
class AntHill : public Energy{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int numColony, Compiler* comp);
    ~AntHill();
    virtual void doSomething();
    
    virtual bool isColony() const;
    
    virtual int getColony() const;
    
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
    void addStun();
    void addSleep();
    void decTime();
    
    bool isInactive() const;

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
    
    bool isAnt() const;
    
    virtual int getColony() const;
    
private:
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
    
    virtual bool isGHopper() const;
    virtual bool isDanger() const;
    
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
};

//===AdultGrasshopper -> Grasshopper -> Mover -> Energy  -> Actor -> GraphObject
class AdultGrasshopper : public Grasshopper{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    
    virtual bool tryGetPoisoned();
    virtual bool tryGetBitten(int damage);
    virtual bool tryGetStunned();
};

#endif // ACTOR_H_

























