#include "btree.h"

#include <map>
#include <vector>
using namespace std;

// TODO: For ease to debug, start with a simple 2-3 tree
const int max_size = 3;
const int min_size = 2;

class btree_node;

struct insert_result
{
    bool succeed;
    bool split;
};

struct split_result
{
    btree_node* sibling;
    int key;
};

class btree_node
{
public:
    virtual ~btree_node();
    virtual insert_result insert(int key, int value) = 0;
    virtual split_result split() = 0;
};

class btree_internal_node : public btree_node
{
public:
    btree_internal_node(int key, btree_node* left, btree_node* right);
    ~btree_internal_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
private:
    vector<int> keys;
    vector<btree_node*> children;
};

class btree_leaf_node : public btree_node
{
public:
    virtual ~btree_leaf_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
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

insert_result btree_leaf_node::insert(int key, int value)
{
    insert_result result;

    auto probe = this->entries.find(key);
    if (probe == this->entries.end())
    {
        result.succeed = true;
        this->entries.insert(pair<int, int>(key, value));
        result.split = (entries.size() == max_size + 1);
        return result;
    }
    else
    {
        result.succeed = false;
        return result;
    }
}

split_result btree_leaf_node::split()
{
    split_result result;
    btree_leaf_node* sibling = new btree_leaf_node();
    int i = 0;
    for (auto iter = this->entries.begin(); i < min_size; iter++, i++)
    {
        sibling->entries.insert(*iter);
    }
    for (auto iter = sibling->entries.begin(); iter != sibling->entries.end(); iter++)
    {
        this->entries.erase(iter->first);
    }

    result.sibling = sibling;
    result.key = this->entries.begin()->first;
    return result;
}

btree_internal_node::btree_internal_node(int key, btree_node* left, btree_node* right)
{
    this->keys.push_back(key);
    this->children.push_back(left);
    this->children.push_back(right);
}

btree_internal_node::~btree_internal_node()
{

}

insert_result btree_internal_node::insert(int key, int value)
{
    insert_result result;
    for (int upper_index = 0; upper_index <= this->keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->keys.size()) || (key < this->keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            insert_result children_insert_result = this->children[upper_index]->insert(key, value);
            if (children_insert_result.succeed)
            {
                result.succeed = true;
                if (children_insert_result.split)
                {
                    split_result children_split_result = this->children[upper_index]->split();
                    this->children.insert(this->children.begin() + upper_index, children_split_result.sibling);
                    this->keys.insert(this->keys.begin() + upper_index, children_split_result.key);
                    result.split = this->keys.size() == max_size;
                }
                else
                {
                    result.split = false;
                }
            }
            else
            {
                result.succeed = false;
            }

            break;
        }
    }

    return result;
}

split_result btree_internal_node::split()
{
    // TODO: Implementation
    split_result result;
    result.key = 0;
    return result;
}

bool btree_impl::insert(int key, int value)
{
    if (this->m_root == nullptr)
    {
        this->m_root = new btree_leaf_node();
    }

    insert_result result = this->m_root->insert(key, value);

    if (result.succeed)
    {
        if (result.split)
        {
            split_result split_result = this->m_root->split();
            this->m_root = new btree_internal_node(split_result.key, split_result.sibling, this->m_root);

            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}