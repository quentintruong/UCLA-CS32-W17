//
//  testnewSequence.cpp
//  HW1
//
//  Created by Quentin Truong on 1/19/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "newSequence.h"
#include <iostream>
#include <cassert>

int main()
{
    Sequence s;
    assert(s.empty());
    
    std::cout << std::endl << "INSERT 2 ARG";
    
    s.insert(0, 99);
    s.insert(0, 0);
    s.insert(1, 1);
    s.insert(2, 2);
    s.insert(3, 3);
    s.dump();
    
    s.insert(-1, 99);
    s.insert(6, 99);
    s.dump();
    
    std::cout << std::endl << "INSERT 1 ARG";
    
    assert(s.insert(0) == 0);
    s.insert(100);
    s.insert(98);
    s.insert(98);
    s.insert(0);//not negative rn bc unsigned long, but for signed int, use negative
    s.dump();
    
    std::cout << std::endl  << "ERASE";
    
    assert(s.erase(0) == true);
    assert(s.erase(-1) == false);
    assert(s.erase(9) == false);
    s.dump();
    
    std::cout << std::endl << "REMOVE";
    
    assert(s.remove(0) == 2);
    s.dump();
    
    std::cout << std::endl << "GET";
    
    ItemType a = 0;
    assert(s.get(6, a) == true && a == 100);
    assert(s.get(7, a) == false && a == 100);
    assert(s.get(0, a) == true && a == 1);
    
    s.dump();
    
    std::cout << std::endl << "FIND";
    
    assert(s.find(69) == -1);
    assert(s.find(2) == 1);
    s.dump();
    
    std::cout << std::endl << "SWAP";
    
    Sequence ss;
    s.swap(ss);
    s.dump();
    ss.dump();
    
    std::cout << std::endl << "COPY CONSTRUCTOR";
    
    Sequence sss(ss);
    sss.insert(0);
    ss.dump();
    sss.dump();
    
    std::cout << std::endl << "ASSIGN OP";
    
    ss = sss;
    sss.insert(0);
    ss.dump();
    sss.dump();
    ss = ss;
    ss.dump();
}

/*
//STRING TESTER
#include "newSequence.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    assert(s.insert(0, "lavash"));
    assert(s.insert(0, "tortilla"));
    assert(s.size() == 2);
    ItemType x = "injera";
    assert(s.get(0, x) && x == "tortilla");
    assert(s.get(1, x) && x == "lavash");
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
*/
