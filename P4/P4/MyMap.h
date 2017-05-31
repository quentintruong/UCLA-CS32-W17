// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include <iostream>
#include <string>

template<typename KeyType, typename ValueType>
struct Node{
public:
    Node(KeyType key, ValueType value) : m_key(key), m_value(value), left(nullptr), right(nullptr){};
    KeyType m_key;
    ValueType m_value;
    Node* left;
    Node* right;
};

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    void clearNode(Node<KeyType, ValueType>* curr);
    void associateNode(const KeyType& key, const ValueType& value, Node<KeyType, ValueType>* curr);
    const ValueType* findValue(const KeyType& key, Node<KeyType, ValueType>* curr) const;

    int m_numNodes;
    Node<KeyType, ValueType>* m_root;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() : m_numNodes(0), m_root(nullptr){}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap(){
    clear();
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(){
    if (m_root == nullptr)
        return;
    clearNode(m_root->left);
    clearNode(m_root->right);
    //std::cerr << "Deleting: " << m_root->m_key << std::endl;
    delete m_root;
}
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const{
    return m_numNodes;
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value){
    if (m_root == nullptr){
        m_root = new Node<KeyType, ValueType>(key, value);
        m_numNodes++;
    }
    associateNode(key, value, m_root);
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const{
    return findValue(key, m_root);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associateNode(const KeyType& key, const ValueType& value, Node<KeyType, ValueType>* curr){
    if (key < curr->m_key){
        if (curr->left == nullptr){
            curr->left = new Node<KeyType, ValueType>(key, value);
            m_numNodes++;
        }
        else
            associateNode(key, value, curr->left);
        return;
    }
    else if (key > curr->m_key){
        if (curr->right == nullptr){
            curr->right = new Node<KeyType, ValueType>(key, value);
            m_numNodes++;
        }
        else
            associateNode(key, value, curr->right);
        return;
    }
    else{// if (key == curr->m_key)
        //std::cerr << "== ";
        curr->m_value = value;
        return;
        //std::cerr << curr->m_key << ": " << curr->m_value << std::endl;
    }
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clearNode(Node<KeyType, ValueType>* curr){
    if (curr == nullptr)
        return;
    clearNode(curr->left);
    clearNode(curr->right);
    //std::cerr << "Deleting: " << curr->m_key << std::endl;
    delete curr;
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::findValue(const KeyType& key, Node<KeyType, ValueType>* curr) const{
    if (curr == nullptr)
        return nullptr;
    
    if (key == curr->m_key)
        return &curr->m_value;
    
    const ValueType* temp;
    if (key < curr->m_key){
        temp = findValue(key, curr->left);
    }
    else if (key > curr->m_key){
        temp = findValue(key, curr->right);
    }
    return temp;
}

#endif





