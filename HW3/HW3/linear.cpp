//
//  linear.cpp
//  HW3
//
//  Created by Quentin Truong on 2/10/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <cassert>
#include <iostream>

bool somePredicate(double x)//returns true if x is 0
{
    return x == 0;
}

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if (n < 1)
        return true;
    
    return somePredicate(a[0]) && allTrue(a + 1, n-1);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if (n < 1)
        return 0;
    
    return !somePredicate(a[0]) + countFalse(a + 1, n-1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if (n < 1)
        return -1;
    
    bool curr = somePredicate(a[0]);
    if (curr == false)
        return 0;
    
    int pos = firstFalse(a+1,n-1);
    if (pos == -1)
        return pos;
    
    return pos + 1;
    
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n < 1)
        return -1;
    if (n == 1)
        return 0;
    
    int minPos = 1+indexOfMin(a+1,n-1);
    
    if (a[0] > a[minPos])
        return minPos;
    else
        return 0;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 < 0 || n2 < 0)
        return false;
    
    if (n2 == 0)
        return true;
    
    if (n1 == 0)
        return false;
    
    if (a1[0] == a2[0])
        return true && includes(a1+1, n1-1, a2+1, n2-1);
    else
        return includes(a1+1, n1-1, a2, n2);
    return false;
}
/*
int main()
{
    double a[] = {0.0};
    double b[] = {1.0};
    double c[] = {1.0, 0.0};
    double d[] = {1.0, 1.0, 1.0};
    double e[] = {0.0, 0.0, 0.0, 0.0};
    double f[] = {};
    
    assert(allTrue(a, 1) == true);
    assert(allTrue(b, 1) == false);
    assert(allTrue(c, 2) == false);
    assert(allTrue(d, 3) == false);
    assert(allTrue(e, 4) == true);
    assert(allTrue(f, 0) == true);
    
    assert(countFalse(a, 1) == 0);
    assert(countFalse(b, 1) == 1);
    assert(countFalse(c, 2) == 1);
    assert(countFalse(d, 3) == 3);
    assert(countFalse(e, 4) == 0);
    assert(countFalse(f, 0) == 0);
    
    assert(firstFalse(a, 1) == -1);//0 == 0 true
    assert(firstFalse(b, 1) == 0);//1 == 0 false
    assert(firstFalse(c, 2) == 0);
    assert(firstFalse(d, 3) == 0);
    assert(firstFalse(e, 4) == -1);
    assert(firstFalse(f, 0) == -1);
    
    double g[] = {0.0};
    double h[] = {0.0, 1.0, 2.0, 3.0, 4.0};
    double i[] = {4.0, 3.0, 2.0, 1.0, 0.0};
    double j[] = {1.0, 2.0, 0.0, 5.0, 4.0};
    double k[] = {};
    
    assert(indexOfMin(g, 1) == 0);
    assert(indexOfMin(h, 5) == 0);
    assert(indexOfMin(i, 5) == 4);
    assert(indexOfMin(j, 5) == 2);
    assert(indexOfMin(k, 0) == -1);
    
    double l[] = {10, 50, 40, 20, 50, 40, 30};
    double ll[] = {50, 20, 30};
    double lll[] = {50, 40, 40};
    double llll[] = {50, 30, 20};
    double lllll[] = {10, 20, 20};
    double llllll[] = {};
    
    assert(includes(l, 7, ll, 3) == true);
    assert(includes(l, 7, lll, 3) == true);
    assert(includes(l, 7, llll, 3) == false);
    assert(includes(l, 7, lllll, 3) == false);
    assert(includes(l, 7, llllll, 0) == true);
    assert(includes(llllll, 0, l, 7) == false);
    assert(includes(llllll, -1, l, -1) == false);
    assert(includes(ll, 3, l, 7) == false);
    
    assert((bool) 1 == true);
    assert((bool) 0 == false);
    assert((bool) 5 == true);
    assert((bool) !5 == false);
    
    std::cout << "Pass" << std::endl;
}*/




