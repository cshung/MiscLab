#include "test.h"

class fenwick_tree
{
public:
    fenwick_tree(int capacity);
    ~fenwick_tree();
    void update(int index, int delta);
    int running_sum(int index) const;
private:
    int m_capacity;
    int* m_elements;
};

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
    while (tree_index > 0)
    {
        int array_index = tree_index - 1;
        this->m_elements[array_index] += delta;
        tree_index = tree_index - (tree_index & -tree_index);
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

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    // min_max_heap_test();
    fenwick_tree tree(10);
    for (int i = 0; i < 10; i++)
    {
        tree.update(i, i);
    }
    cout << tree.running_sum(9) << endl;
    return 0;
}