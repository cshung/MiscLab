#pragma once

class btree_impl;

class btree
{
public:
    btree();
    ~btree();
    bool insert(int key, int value);
    bool select(int key, int* result) const;
    bool remove(int key);

    void print() const;
private:
    btree_impl* m_impl;
};
