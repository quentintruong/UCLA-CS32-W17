//
//  newSequence.cpp
//  HW1
//
//  Created by Quentin Truong on 1/19/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "newSequence.h"
#include <iostream>

Sequence::Sequence()
{
    m_array = new ItemType[DEFAULT_MAX_ITEMS];
    m_max = DEFAULT_MAX_ITEMS;
    m_size = 0;
}


Sequence::Sequence(int max)
{
    m_array = new ItemType[max];
    m_max = max;
    m_size = 0;
}


Sequence::Sequence(const Sequence& other)
{
    m_array = new ItemType[other.m_max];
    m_max = other.m_max;
    m_size = other.m_size;
    
    for (int i = 0; i < m_size; i++)
         m_array[i] = other.m_array[i];
}

Sequence::~Sequence(){
    delete[] m_array;
}

Sequence& Sequence::operator= (const Sequence& other)
{
    delete[] m_array;
    m_array = new ItemType[other.m_max];
    m_max = other.m_max;
    m_size = other.m_size;
    
    for (int i = 0; i < m_size; i++)
        m_array[i] = other.m_array[i];
    
    return (*this);
}

bool Sequence::empty() const
{
    if (m_size != 0)
        return false;
    return true;
}

int Sequence::size() const
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > m_size || pos >= m_max)
        return false;
    
    int temp_pos = m_size;
    while (temp_pos > pos){
        m_array[temp_pos] = m_array[temp_pos - 1];
        temp_pos--;
    }
    m_size++;
    m_array[pos] = value;
    return true;
}

int Sequence::insert(const ItemType& value)
{
    if (m_size == m_max)
        return -1;
    int p = 0;
    if (m_size != 0)
        while (value > m_array[p] && p < m_size)
            p++;
    insert(p, value);
    return p;
}

bool Sequence::erase(int pos)
{
    if (pos < 0 || pos >= m_size)
        return false;
    
    int temp_pos = pos;
    m_size--;
    
    while (temp_pos < m_size){
        m_array[temp_pos] = m_array[temp_pos + 1];
        temp_pos++;
    }
    return true;
}

int Sequence::remove(const ItemType& value)
{
    int inc = 0;
    for (int i = 0; i < m_size; i++){
        if (m_array[i] == value){
            erase(i);
            i--;
            inc++;
        }
    }
    return inc;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= m_size)
        return false;
    
    value = m_array[pos];
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= m_size)
        return false;
    
    m_array[pos] = value;
    return true;
}

int Sequence::find(const ItemType& value) const
{
    int p = -1;
    for (int i = 0; i < m_size; i++){
        if (m_array[i] == value){
            p = i;
        }
        continue;
    }
    return p;
}

void Sequence::swap(Sequence& other)
{
    int temp_size = m_size;
    ItemType* temp_pointerArray = m_array;
    
    m_size = other.m_size;
    m_array = other.m_array;
    
    other.m_size = temp_size;
    other.m_array = temp_pointerArray;
}

void Sequence::dump() const
{
    for (int a = 0; a < m_size; a++)
        std::cerr << m_array[a] << " ";
    std::cerr << "Size: " << m_size << std::endl;
}

