// min_max_heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Implements a double ended priority queue so that we can get/delete min and get/delete max fast
// For performance, we will use a backing array of fixed size, we can easily imagine changing this into a vector
template<class T>
class min_max_heap
{
public:
    min_max_heap(int capacity);
    ~min_max_heap();
    bool try_insert()
private:
    int m_capacity;
    T* m_storage;
};

template<class T>
min_max_heap<T>::min_max_heap(int capacity) : m_capacity(capacity), m_storage(new T[capacity])
{
}

template<class T>
min_max_heap<T>::~min_max_heap()
{
    if (this->m_storage != nullptr)
    {
        delete this->m_storage;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    return 0;
}
