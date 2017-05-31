//
//  testSequence.cpp
//  P2
//
//  Created by Quentin Truong on 1/25/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <string>
#include "Sequence.h"

int main(){
    Sequence a;
    assert(a.empty() == true);//empty check when nothing
    assert(a.size() == 0);//size check when nothing
    
    std::cerr << "~~~INSERT~~~" << std::endl;
    
    assert(a.insert(-1, "69") == false);//no neg pos
    assert(a.insert(1, "69") == false);//no beyond pos
    assert(a.insert(0, "4") == true);//actually inserts at all
    assert(a.insert(0, "1") == true);//will add to start of non-trivial seq
    assert(a.insert(1, "2") == true);//will add to end of non-trivial seq
    assert(a.insert("0") == 0);//will find start
    assert(a.insert("3") == 3);//will find mid
    assert(a.insert("5") == 5);//will find end
    
    assert(a.empty() == false);//empty check when stuff
    assert(a.size() == 6);//size check when stuff
    
    a.dump();//visible check
    
    std::cerr << "~~~SET~~~" << std::endl;
    
    Sequence b(a);//copy construc
    assert(b.set(-1, "-1") == false);//no neg pos
    assert(b.set(6, "-1") == false);//no beyond pos
    assert(b.set(0, "-1") == true);//will set beg
    assert(b.set(3, "-1") == true);//will set mid
    assert(b.set(5, "-1") == true);//will set end
    a.dump();//visible check copy construc for independence
    b.dump();//visible check copy construc for independence
    
    std::cerr << "~~~ERASE~~~" << std::endl;
    assert(b.erase(-1) == false);//no neg pos
    assert(b.erase(6) == false);//no beyond pos
    assert(b.erase(0) == true);//no erase beg
    assert(b.erase(2) == true);//no erase mid
    assert(b.erase(3) == true);//no erase end
    b.dump();//visible check
    
    std::cerr << "~~~FIND~~~" << std::endl;
    assert(b.find("-1") == -1);//not found
    assert(b.find("1") == 0);//found beg
    assert(b.find("2") == 1);//found mid
    assert(b.find("4") == 2);//found end
    std::cerr << std::endl;
    
    std::cerr << "~~~REMOVE~~~" << std::endl;
    b.insert("0");
    b.insert("1");
    b.insert("1");
    b.insert("1");
    b.dump();//dumped to compare it w end result
    assert(b.remove("-1") == 0);//not found
    assert(b.remove("0") == 1);//removes beg
    assert(b.remove("1") == 4);//removes multiples
    assert(b.remove("2") == 1);//removes mid
    assert(b.remove("4") == 1);//removes end and all
    b.dump();//visible check

    std::cerr << "~~~GET~~~" << std::endl;
    std::string str = "no";
    assert(a.get(-1, str) == false && str == "no");//no neg pos
    assert(a.get(6, str) == false && str == "no");//no beyond pos
    assert(a.get(0, str) == true && str == "0");//get beg
    assert(a.get(2, str) == true && str == "2");//get mid
    assert(a.get(5, str) == true && str == "5");//get end
    a.dump();//verify const func + helpful to look at
    
    std::cerr << "~~~SWAP~~~" << std::endl;
    a.dump();
    b.dump();
    a.swap(b);//swaps
    a.dump();
    b.dump();
    b.swap(b);//swap safe for itself
    b.dump();
    
    std::cerr << "~~~OPERATOR=~~~" << std::endl;
    a.dump();
    b.dump();
    a = b;//equals
    a.dump();
    b.dump();
    Sequence c;
    a = c;//equals works on empty
    c.dump();
    a.dump();
    a = a;//equals safe for itself
    a.dump();
    
    std::cerr << "~~~SUBSEQUENCE~~~" << std::endl;
    Sequence one;//0 1 2 3
    Sequence two;//1 2
    one.insert("0");
    one.insert("1");
    one.insert("2");
    one.insert("3");
    one.insert("4");
    one.insert("5");
    assert(subsequence(one, two) == -1);//empty seq2
    two.insert("-1");
    assert(subsequence(one, two) == -1);//no k exists
    two.set(0, "0");
    assert(subsequence(one, two) == 0);//finds beginning, length 1
    two.insert("1");
    assert(subsequence(one, two) == 0);//finds beginning, length 2
    two.remove("0");
    assert(subsequence(one, two) == 1);//finds mid, length 1
    two.insert("2");
    assert(subsequence(one, two) == 1);//finds mid, length 2
    two.insert("3");
    two.insert("4");
    two.insert("5");
    assert(subsequence(one, two) == 1);//finds mid, length 5, touching end
    two.set(4, "6");
    assert(subsequence(one, two) == -1);//no k exists, touching end
    two.insert("0");
    assert(subsequence(one, two) == -1);//no k exists, touching beg/end
    two.set(5, "5");
    assert(subsequence(one, two) == 0);//finds beginning, length full, touching beg/end
    Sequence ver;
    Sequence ify;
    ver.insert(0, "30");
    ver.insert(1, "21");
    ver.insert(2, "63");
    ver.insert(3, "42");
    ver.insert(4, "17");
    ver.insert(5, "63");
    ver.insert(6, "17");
    ver.insert(7, "29");
    ver.insert(8, "8");
    ver.insert(9, "32");
    ify.insert(0, "63");
    ify.insert(1, "17");
    ify.insert(2, "29");
    assert(subsequence(ver, ify) == 5);
    ify.set(0, "17");
    ify.set(1, "63");
    assert(subsequence(ver, ify) == -1);
    
    std::cerr << "~~~INTERWEAVE~~~" << std::endl;
    ver.dump();//30 21 63 42 17 63 17 29 8 32
    ify.dump();//17 63 29
    Sequence end;
    interleave(ver, ify, end);//diff length
    end.dump();//30 17 21 63 63 29 42 17 63 17 29 8 32
    interleave(end, ify, end);//aliasing
    end.dump();//30 17 17 63 21 29 63 63 29 42 17 63 17 29 8 32
    interleave(end, end, end);//aliasing and same length
    end.dump();//doubles
    Sequence empty;
    interleave(end, empty, end);//empty, so ret copy
    end.dump();//doubles
    
    
    
    /*
    Sequence a;
    
    assert(a.empty());
    assert(a.size() == 0);
    
    a.insert(0, "Apples"); //Empty
    a.insert(0, "Aaa"); //Insert start
    a.insert(2, "Cheerios"); //Insert end
    a.insert(2, "Bananas"); //Insert middle
    a.insert(4, "Doritos"); //Insert end
    
    a.dump();
    
    assert(a.insert("Abc") == 1);
    assert(a.insert("Aa") == 0);
    assert(a.insert("Elephants") == 7);
    
    a.dump();
    
    //Testing erase
    assert(!a.erase(-1));
    assert(!a.erase(8));
    
    assert(a.erase(0));
    assert(a.erase(1));
    assert(a.erase(5));
    
    a.dump();
    
    //Testing remove
    assert(a.remove("Zebras") == 0);
    assert(a.remove("Aaa") == 1);
    
    a.dump();
    
    a.insert(0, "Remove");
    a.insert(2, "Remove");
    a.insert(6, "Remove");
    
    assert(a.remove("Remove") == 3);
    
    a.dump();
    
    //Testing get
    std::string test;
    assert(a.get(0, test) && test == "Apples");
    assert(a.get(1, test) && test == "Bananas");
    assert(a.get(2, test) && test == "Cheerios");
    assert(a.get(3, test) && test == "Doritos");
    assert(!a.get(-1, test) && test == "Doritos");
    assert(!a.get(4, test) && test == "Doritos");
    
    //Tesing set
    assert(a.set(0, "Anas"));
    assert(a.get(0, test) && test == "Anas");
    assert(a.set(1, "Banas"));
    assert(a.get(1, test) && test == "Banas");
    assert(!a.set(-1, test));
    assert(!a.set(4, test));
    
    
    //Testing find
    a.dump();
    assert(a.find("Anas") == 0);
    assert(a.find("Banas") == 1);
    assert(a.find("Doritos") == 3);
    a.insert(4, "Banas");
    assert(a.find("Banas") == 1);
    
    a.dump();
    
    //Testing Swap
    Sequence b;
    b.insert("Hi");
    a.swap(b);
    b.swap(b);
    
    a.dump();
    b.dump();
    
    //Testing Copy Constructor
    Sequence c(b);
    
    c.dump();
    
    //Testing Assignment Operator
    a = c;
    a = a;
    a.dump();
    
    b.set(0, "Banas");
    c.set(0, "Canas");
    assert(a.get(0, test) && test == "Anas");
    assert(b.get(0, test) && test == "Banas");
    assert(c.get(0, test) && test == "Canas");
    
    //Testing new Functions
    Sequence uno;
    Sequence dos;
    uno.insert("5");
    uno.insert("6");
    uno.insert("7");
    uno.insert("8");
    uno.insert("9");
    uno.dump();
    
    dos.insert("7");
    dos.insert("8");
    dos.insert("9");
    dos.dump();
    
    assert(subsequence(uno, dos) == 2);
    dos.set(2, "8");
    assert(subsequence(uno, dos) == -1);
    dos.set(0, "5");
    dos.set(1, "6");
    dos.set(2, "7");
    assert(subsequence(uno, dos) == 0);
    dos = uno;
    assert(subsequence(uno, dos) == 0);
    
    Sequence test1;
    Sequence test2;
    
    test1.insert(0, "30");
    test1.insert(1, "21");
    test1.insert(2, "63");
    test1.insert(3, "42");
    test1.insert(4, "17");
    test1.insert(5, "63");
    
    test2.insert(0, "42");
    test2.insert(1, "63");
    test2.insert(2, "84");
    test2.insert(3, "19");
    
    Sequence answer;
    
    interleave(test1, test2, answer); //This better work
    
    assert(answer.get(0, test) && test == "30");
    assert(answer.get(2, test) && test == "21");
    assert(answer.get(8, test) && test == "17");
    assert(answer.get(9, test) && test == "63");
    
    answer.dump();
    */
}


//TYPES TESTER MAIN
/*
#include "Sequence.h"

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

static_assert(std::is_default_constructible<Sequence>::value,
              "Sequence must be default-constructible.");
static_assert(std::is_copy_constructible<Sequence>::value,
              "Sequence must be copy-constructible.");

void thisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Sequence::operator=,  Sequence& (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::empty,      bool (Sequence::*)() const);
    CHECKTYPE(&Sequence::size,       int  (Sequence::*)() const);
    CHECKTYPE(&Sequence::insert,     bool (Sequence::*)(int, const ItemType&));
    CHECKTYPE(&Sequence::insert,     int (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::erase,      bool (Sequence::*)(int));
    CHECKTYPE(&Sequence::remove,     int  (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::get,	 bool (Sequence::*)(int, ItemType&) const);
    CHECKTYPE(&Sequence::set,	 bool (Sequence::*)(int, const ItemType&));
    CHECKTYPE(&Sequence::find,       int  (Sequence::*)(const ItemType&) const);
    CHECKTYPE(&Sequence::swap,       void (Sequence::*)(Sequence&));
    CHECKTYPE(subsequence, int  (*)(const Sequence&, const Sequence&));
    CHECKTYPE(interleave,  void (*)(const Sequence&, const Sequence&, Sequence&));
}

int main()
{}
*/

//STRING TESTER MAIN
/*
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
    assert(s.get(0, x)  &&  x == "tortilla");
    assert(s.get(1, x)  &&  x == "lavash");
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
*/

//UNSIGNED LONG MAIN
/*
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    assert(s.insert(0, 10));
    assert(s.insert(0, 20));
    assert(s.size() == 2);
    ItemType x = 999;
    assert(s.get(0, x)  &&  x == 20);
    assert(s.get(1, x)  &&  x == 10);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
*/
