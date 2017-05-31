//
//  ScoreList.cpp
//  HW1
//
//  Created by Quentin Truong on 1/19/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "ScoreList.h"
#include <iostream>

ScoreList::ScoreList()
{
}

bool ScoreList::add(unsigned long score)
{
    if (score < 0 || score > 100)
        return false;
    
    m_seq.insert(score);

    return true;
}

bool ScoreList::remove(unsigned long score)
{
    int p = m_seq.find(score);
    if (p != -1){
        m_seq.erase(p);
        return true;
    }
    return false;
}

int ScoreList::size() const
{
    return m_seq.size();
}

unsigned long ScoreList::minimum() const
{
    if (size() == 0)
        return NO_SCORE;
    
    unsigned long low = 0;
    m_seq.get(0, low);
    return low;
}

unsigned long ScoreList::maximum() const
{
    if (size() == 0)
        return NO_SCORE;
    
    unsigned long high = 0;
    m_seq.get(size() - 1, high);
    return high;
}




