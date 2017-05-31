//
//  Sequence.cpp
//  HW1
//
//  Created by Quentin Truong on 1/18/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "Sequence.h"
#include <iostream>

Sequence::Sequence()
{
    m_size = 0;
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
    if (pos < 0 || pos > m_size || pos >= DEFAULT_MAX_ITEMS)
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
    if (m_size == 200)
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
    ItemType temp_elem;
    
    m_size = other.m_size;
    other.m_size = temp_size;
    
    for (int i = 0; i < other.m_size; i++){
        temp_elem = m_array[i];
        m_array[i] = other.m_array[i];
        other.m_array[i] = temp_elem;
    }
}

void Sequence::dump() const
{
    for (int a = 0; a < m_size; a++)
        std::cerr << m_array[a] << " ";
    std::cerr << "Size: " << m_size << std::endl;
}
