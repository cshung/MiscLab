// min_max_heap.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <algorithm>
#include <iostream>
using namespace std;

class min_max_heap
{
public:
    min_max_heap(int capacity);
    ~min_max_heap();
    bool try_insert(double value);
    bool try_get_min(double* min_value) const;
    bool try_get_max(double* max_value) const;
private:
    int m_capacity;
    int m_size;
    double* m_storage;

    bool is_min_node(int node_number);
    bool has_parent_node(int node_number);
    int parent_node(int node_number);
    void bubble_up_min_node(int node_number);
    void bubble_up_max_node(int node_number);
};

min_max_heap::min_max_heap(int capacity) : m_capacity(capacity), m_size(0), m_storage(new double[capacity])
{
}

min_max_heap::~min_max_heap()
{
    if (this->m_storage != nullptr)
    {
        delete this->m_storage;
    }
}

bool min_max_heap::try_insert(double value)
{
    if (this->m_size == this->m_capacity)
    {
        return false;
    }

    // Step 1: Place the value in the last element
    this->m_storage[this->m_size] = value;
    this->m_size++;
    int node_number = this->m_size;

    // Step 2: Bubbling the value up
    if (this->is_min_node(node_number))
    {
        this->bubble_up_min_node(node_number);
    }
    else
    {
        this->bubble_up_max_node(node_number);
    }
    
    return true;
}

bool min_max_heap::try_get_min(double* min_value) const
{
    if (min_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else
    {
        *min_value = this->m_storage[0];
        return true;
    }
}

bool min_max_heap::try_get_max(double* max_value) const
{
    if (max_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else
    {
        if (this->m_size == 1)
        {
            *max_value = this->m_storage[0];
        }
        else if (this->m_size == 2)
        {
            *max_value = this->m_storage[1];
        }
        else
        {
            *max_value = max(this->m_storage[1], this->m_storage[2]);
        }

        return true;
    }
}

bool min_max_heap::has_parent_node(int node_index)
{
    return node_index != 1;
}

int min_max_heap::parent_node(int node_index)
{
    return node_index / 2;
}

bool min_max_heap::is_min_node(int node_index)
{
    if (node_index == 1)
    {
        return true;
    }
    else
    {
        return !(this->is_min_node(this->parent_node(node_index)));
    }
}

void min_max_heap::bubble_up_min_node(int node_number)
{
    if (this->has_parent_node(node_number))
    {
        int max_node_number = this->parent_node(node_number);
        if (this->m_storage[max_node_number - 1] < this->m_storage[node_number - 1])
        {
            swap(this->m_storage[max_node_number - 1], this->m_storage[node_number - 1]);
            this->bubble_up_max_node(max_node_number);
        }
        else if (this->has_parent_node(max_node_number))
        {
            int min_node_number = this->parent_node(max_node_number);
            if (this->m_storage[min_node_number - 1] > this->m_storage[node_number - 1])
            {
                swap(this->m_storage[min_node_number - 1], this->m_storage[node_number - 1]);
                this->bubble_up_min_node(min_node_number);
            }
        }
    }
}

void min_max_heap::bubble_up_max_node(int node_number)
{
    if (this->has_parent_node(node_number))
    {
        int min_node_number = this->parent_node(node_number);
        if (this->m_storage[min_node_number - 1] > this->m_storage[node_number - 1])
        {
            swap(this->m_storage[min_node_number - 1], this->m_storage[node_number - 1]);
            this->bubble_up_min_node(min_node_number);
        }
        else if (this->has_parent_node(min_node_number))
        {
            int max_node_number = this->parent_node(min_node_number);
            if (this->m_storage[max_node_number - 1] < this->m_storage[node_number - 1])
            {
                swap(this->m_storage[max_node_number - 1], this->m_storage[node_number - 1]);
                this->bubble_up_max_node(max_node_number);
            }
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    min_max_heap heap(20);
    heap.try_insert(30);
    heap.try_insert(20);
    heap.try_insert(10);
    heap.try_insert(5);
    double min;
    double max;
    heap.try_get_min(&min);
    heap.try_get_max(&max);
    cout << min << endl;
    cout << max << endl;
    return 0;
}
