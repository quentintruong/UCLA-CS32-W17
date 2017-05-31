//
//  testScoreList.cpp
//  HW1
//
//  Created by Quentin Truong on 1/19/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "ScoreList.h"

int main()
{
    ScoreList sl;
    assert(sl.size() == 0);
    
    std::cout << std::endl << "ADD";
    assert(sl.add(50) == true);
    assert(sl.add(49) == true);
    assert(sl.add(51) == true);
    assert(sl.add(50) == true);
    assert(sl.add(-1) == false);
    assert(sl.add(101) == false);
    
    std::cout << std::endl << "REMOVE";
    assert(sl.remove(50) == true);
    assert(sl.remove(69) == false);
    
    std::cout << std::endl << "MIN/MAX";
    assert(sl.minimum() == 49);
    assert(sl.maximum() == 51);
    
    std::cout << std::endl << "AUTO COPY CONST";
    ScoreList sl2(sl);
    assert(sl2.minimum() == 49);
    assert(sl2.maximum() == 51);
    
    std::cout << std::endl << "AUTO COPY ASSIGN";
    ScoreList sl3;
    sl3 = sl;
    assert(sl3.minimum() == 49);
    assert(sl3.maximum() == 51);
}
