#pragma once

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
