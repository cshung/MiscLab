#include "btree.h"

#include <map>
using namespace std;

class btree_node
{
public:
    virtual ~btree_node();
    virtual btree_node* insert(int key, int value) = 0;
};

class btree_leaf_node : public btree_node
{
public:
    virtual ~btree_leaf_node();
    virtual btree_node* insert(int key, int value);
private:
    // TODO: Ideally, this should work on the memory page directly
    map<int, int> entries;
};

class btree_impl
{
public:
    bool insert(int key, int value);
private:
    btree_node* m_root;
};

btree::btree()
{
    this->m_impl = new btree_impl();
}

btree::~btree()
{
    delete this->m_impl;
}

bool btree::insert(int key, int value)
{
    return this->m_impl->insert(key, value);
}

btree_node::~btree_node()
{
}

btree_leaf_node::~btree_leaf_node()
{
}

btree_node* btree_leaf_node::insert(int key, int value)
{
    auto probe = this->entries.find(key);
    if (probe == this->entries.end())
    {
        // Assuming the node is not full
        this->entries.insert(pair<int, int>(key, value));
        return this;
    }
    else
    {
        return nullptr;
    }
}

bool btree_impl::insert(int key, int value)
{
    if (this->m_root == nullptr)
    {
        this->m_root = new btree_leaf_node();
    }

    btree_node* result = this->m_root->insert(key, value);

    if (result == nullptr)
    {
        return false;
    }
    else
    {
        this->m_root = result;
        return true;
    }
}