#include "fenwick_tree.h"

fenwick_tree::fenwick_tree(int capacity) : m_capacity(capacity)
{
    this->m_elements = new int[this->m_capacity];
    for (int i = 0; i < this->m_capacity; i++)
    {
        this->m_elements[i] = 0;
    }
}

fenwick_tree::~fenwick_tree()
{
    delete this->m_elements;
}

void fenwick_tree::update(int index, int delta)
{
    int tree_index = index + 1;
    while (tree_index <= this->m_capacity)
    {
        int array_index = tree_index - 1;
        this->m_elements[array_index] += delta;
        tree_index = tree_index + (tree_index & -tree_index);
    }
}

int fenwick_tree::running_sum(int index) const
{
    int sum = 0;
    int tree_index = index + 1;
    while (tree_index > 0)
    {
        int array_index = tree_index - 1;
        sum += this->m_elements[array_index];
        tree_index = tree_index - (tree_index & -tree_index);
    }

    return sum;
}