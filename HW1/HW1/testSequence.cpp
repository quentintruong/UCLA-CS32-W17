//
//  main.cpp
//  HW1
//
//  Created by Quentin Truong on 1/18/17.
//  Copyright © 2017 QT. All rights reserved.
//

//TESTER
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());
    
    s.insert(0, 99);
    s.insert(0, 0);
    s.insert(1, 1);
    s.insert(2, 2);
    s.insert(3, 3);
    s.dump();
    
    std::cout << std::endl;
    
    s.insert(-1, 99);
    s.insert(6, 99);
    s.dump();
    
    std::cout << std::endl;
    
    assert(s.insert(0) == 0);
    s.insert(100);
    s.insert(98);
    s.insert(98);
    s.insert(0);//not negative rn bc unsigned long, but for signed int, use negative
    s.dump();
    
    std::cout << std::endl;
    
    assert(s.erase(0) == true);
    assert(s.erase(-1) == false);
    assert(s.erase(9) == false);
    s.dump();
    
    std::cout << std::endl;
    
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
    ss.insert(5);
    s.dump();
    ss.dump();
    s.swap(ss);
    s.dump();
    ss.dump();
    
    std::cout << std::endl << "AUTO COPY CONSTRUCTOR";
    
    Sequence sss(ss);
    sss.insert(0);
    ss.dump();
    sss.dump();
    
    std::cout << std::endl << "AUTO ASSIGN OP";
    
    ss = sss;
    sss.insert(0);
    ss.dump();
    sss.dump();
    
    
    
}

/*
//OTHER STRING TESTER
#include "Sequence.h"
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

//STRING TESTER
/*
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find("laobing") == -1);
    s.insert("laobing");
    assert(s.size() == 1  &&  s.find("laobing") == 0);
    cout << "Passed all tests" << endl;
}
*/


//INCOMPLETE TESTER
/*
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    s.insert(42);
    assert(s.size() == 1  &&  s.find(42) == 0);
    cout << "Passed all tests" << endl;
}*/
