#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject{
public:
    Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, int depth, bool isObstacle);
    virtual void doSomething() = 0;
    virtual bool isDead() const;
    virtual bool isObstacle() const;
protected:
    std::vector<int> currentPos() const;
    std::vector<int> nextPos() const;
    StudentWorld* getSW() const;
private:
    StudentWorld* m_sw;
    bool m_isObstacle;
};

class Animate : public Actor{
public:
    Animate(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, int depth, int energy, bool isObstacle);
    virtual void doSomething() = 0;
    bool isDead() const;
private:
    void changeEnergy(int diff);
    int m_energy;
    bool isAsleep;
    bool isStunned;
};

class Pebble : public Actor{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
private:
};

class BabyGrasshopper : public Animate{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
private:
    void changeDirSteps();
    int steps;
};

#endif // ACTOR_H_
