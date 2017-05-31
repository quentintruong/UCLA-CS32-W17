//
//  Sequence.cpp
//  P2
//
//  Created by Quentin Truong on 1/25/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "Sequence.h"

//default constructor
Sequence::Sequence(){
    m_head = new Node();//dummy head
    m_head->prev = nullptr;
    
    m_tail = new Node();//dummy tail
    m_tail->next = nullptr;
    
    m_tail->prev = m_head;
    m_head->next = m_tail;
    
    m_size = 0;
}

//copy constructor
Sequence::Sequence(const Sequence& other){
    m_head = new Node();
    m_head->prev = nullptr;
    
    m_tail = new Node();
    m_tail->next = nullptr;
    
    Node* temp = other.m_head->next;
    Node* prev = m_head;
    
    while (temp->next != nullptr){//copying values
        Node* m_node = new Node();
        
        m_node->prev = prev;
        prev->next = m_node;
        m_node->value = temp->value;
        
        prev = m_node;
        temp = temp->next;
    }
    prev->next = m_tail;
    m_tail->prev = prev;
    
    m_size = other.m_size;
}

//destructor
Sequence::~Sequence(){
    Node* current = m_head;
    while (current->next != nullptr){
        Node* temp = current->next;
        delete current;
        current = temp;
    }
    delete current;//dealloc m_tail
}

//override =
Sequence& Sequence::operator=(const Sequence& other){
    if (this != &other){//catch for operator= to self
        Sequence temp(other);
        swap(temp);
    }
    return *this;
}

//returns true if empty
bool Sequence::empty() const{
    if (m_size == 0)
        return true;
    return false;
}

//returns size of seq
int Sequence::size() const{
    return m_size;
}

//inserts value at pos; returns false if pos is out of range
bool Sequence::insert(int pos, const ItemType& value){
    if (pos < 0 || pos > m_size)
        return false;
    
    Node* temp = m_head;
    for (int i = 0; i < pos; i++){//iterates onto node prev to where value is to be inserted
        temp = temp->next;
    }
    
    Node* m_node = new Node();
    m_node->prev = temp;
    m_node->next = temp->next;
    
    temp->next->prev = m_node;
    temp->next = m_node;
    
    m_node->value = value;
    
    m_size++;
    
    return true;
}

//inserts value in a numerically ordered way
int Sequence::insert(const ItemType& value){
    int p = 0;
    Node* temp = m_head->next;
    
    while (value > temp->value && p < m_size){//ordering
        temp = temp->next;
        p++;
    }
    
    insert(p, value);
    return p;
}

//removes the node at pos
bool Sequence::erase(int pos){
    if (pos < 0 || pos >= m_size)
        return false;
    
    Node* current = m_head;
    for (int i = 0; i <= pos; i++){//iterate onto node
        current = current->next;
    }
    
    current->prev->next = current->next;//set the prev node's next to next node (skipping current)
    current->next->prev = current->prev;//set the next node's prev to prev node (skipping current)
    
    delete current;
    m_size--;

    return true;
}

//removes all nodes with value == the param
int Sequence::remove(const ItemType& value){
    int inc = 0;
    
    while (find(value) != -1){//runs until cannot find it anymore
        erase(find(value));
        inc++;
    }
    
    return inc;
}
bool Sequence::get(int pos, ItemType& value) const{
    if (pos < 0 || pos >= m_size)
        return false;
    
    Node* current = m_head;
    for (int i = 0; i <= pos; i++){//iterate onto node
        current = current->next;
    }
    
    value = current->value;
    
    return true;
}

//sets node's value at pos to the param
bool Sequence::set(int pos, const ItemType& value){
    if (pos < 0 || pos >= m_size)
        return false;
    
    Node* current = m_head;
    for (int i = 0; i <= pos; i++){//iterate onto node
        current = current->next;
    }
    
    current->value = value;
    
    return true;
}

//returns first node's pos with value == param
int Sequence::find(const ItemType& value) const{
    int p = 0;
    Node* current = m_head->next;
    
    while (current->next != nullptr){
        if (current->value == value)
            return p;
        else{
            current = current->next;
            p++;
        }
    }

    return -1;//hit the end w/o finding
}

//swaps sequences data
void Sequence::swap(Sequence& other){
    Node* temp_head = m_head;
    m_head = other.m_head;
    other.m_head = temp_head;
    
    int temp_size = m_size;
    m_size = other.m_size;
    other.m_size = temp_size;
}

//cerr's out data for testing
void Sequence::dump() const{
    int verifyLinkage = 0;
    Node* current = m_head;
    while (current->next != nullptr){
        if (current != current->next->prev)
            verifyLinkage++;
        
        std::cerr << "ADDRESS: " << current;
        std::cerr << " PREV: " << current->prev;
        std::cerr << " NEXT: " << current->next;
        std::cerr << " VALUE: " << current->value << std::endl;
        
        current = current->next;
    }
    std::cerr << "ADDRESS: " << current;
    std::cerr << " PREV: " << current->prev;
    std::cerr << " NEXT: " << current->next;
    std::cerr << " VALUE: " << current->value << std::endl;
    
    std::cerr << "SIZE: " << m_size << std::endl;
    
    std::cerr << "verifyLinkage: " << verifyLinkage << std::endl << std::endl;
}

//returns pos of start of seq2 in seq1
int subsequence(const Sequence& seq1, const Sequence& seq2){
    if (seq2.empty())
        return -1;
    
    int k = 0;//iterate seq1
    int n = 1;//iterate seq2 and a part of seq1
    
    while (k <= seq1.size() - seq2.size()){//only runs loop until the point at which it is no longer possible for seq2 to appear
        n = 1;
        ItemType temp1;
        ItemType temp2;
        seq1.get(k, temp1);
        seq2.get(0, temp2);
        
        if (temp1 == temp2){
            while (temp1 == temp2 && n < seq2.size()){//while seq1 == seq2 and seq2 still has something in it
                seq1.get(k + n, temp1);
                seq2.get(n, temp2);
                n++;
            }
            if (temp1 == temp2 && n == seq2.size())
                return k;//checks last char of seq1, seq2 to actually equal and if n is equal to seq2's size, meaning subseq is found
        }
        k++;
    }
    
    return -1;
}

//combines seq1 and seq2 into result
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result){
    Sequence res;//deals w aliasing
    int k = 0;
    while (k < seq1.size() || k < seq2.size()){//if either seq1 or seq2 has something left in it
        ItemType temp1;
        ItemType temp2;
        bool b1 = seq1.get(k, temp1);
        bool b2 = seq2.get(k, temp2);
        
        if (b1 == true){//only insert if there is actually something there
            res.insert(res.size(), temp1);
        }
        if (b2 == true){
            res.insert(res.size(), temp2);
        }
        k++;
    }
    result = res;
}







