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

struct remove_result
{
    bool succeed;
};

class btree_node
{
public:
    virtual ~btree_node();
    virtual insert_result insert(int key, int value) = 0;
    virtual split_result split() = 0;
    virtual bool select(int key, int* result) = 0;
    virtual remove_result remove(int key) = 0;
};

class btree_internal_node : public btree_node
{
public:
    btree_internal_node(int key, btree_node* left, btree_node* right);
    ~btree_internal_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
    virtual bool select(int key, int* result);
    virtual remove_result remove(int key);
private:
    btree_internal_node();
    vector<int> keys;
    vector<btree_node*> children;
};

class btree_leaf_node : public btree_node
{
public:
    virtual ~btree_leaf_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
    virtual bool select(int key, int* result);
    virtual remove_result remove(int key);
private:
    vector<int> keys;
    vector<int> values;
};

class btree_impl
{
public:
    bool insert(int key, int value);
    bool select(int key, int* result);
    bool remove(int key);
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

bool btree::select(int key, int* result)
{
    return this->m_impl->select(key, result);
}

bool btree::remove(int key)
{
    return this->m_impl->remove(key);
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
    for (size_t upper_index = 0; upper_index <= this->keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->keys.size()) || (key < this->keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            if (lower_index != -1 && key == this->keys[lower_index])
            {
                result.succeed = false;
            }
            else
            {
                this->keys.insert(this->keys.begin() + upper_index, key);
                this->values.insert(this->values.begin() + upper_index, value);
                result.succeed = true;
                result.split = (this->keys.size() == (max_size + 1));
            }

            break;
        }
    }

    return result;
}

split_result btree_leaf_node::split()
{
    split_result result;
    btree_leaf_node* sibling = new btree_leaf_node();
    for (size_t i = min_size; i < this->keys.size(); i++)
    {
        sibling->keys.push_back(this->keys[i]);
        sibling->values.push_back(this->values[i]);
    }
    this->keys.resize(min_size);
    this->values.resize(min_size);

    result.sibling = sibling;
    result.key = sibling->keys[0];
    return result;
}

bool btree_leaf_node::select(int key, int* result)
{
    for (size_t i = 0; i < this->keys.size(); i++)
    {
        if (this->keys[i] == key)
        {
            *result = this->values[i];
            return true;
        }
    }

    return false;
}

remove_result btree_leaf_node::remove(int key)
{
    remove_result result;
    result.succeed = false;
    for (size_t i = 0; i < this->keys.size(); i++)
    {
        if (this->keys[i] == key)
        {
            result.succeed = true;
            this->keys.erase(this->keys.begin() + i);
            this->values.erase(this->values.begin() + i);
            break;
        }
    }

    return result;
}

btree_internal_node::btree_internal_node(int key, btree_node* left, btree_node* right)
{
    this->keys.push_back(key);
    this->children.push_back(left);
    this->children.push_back(right);
}

btree_internal_node::btree_internal_node()
{

}

btree_internal_node::~btree_internal_node()
{

}

insert_result btree_internal_node::insert(int key, int value)
{
    insert_result result;
    for (size_t upper_index = 0; upper_index <= this->keys.size(); upper_index++)
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
                    if (upper_index == this->keys.size())
                    {
                        this->children.push_back(children_split_result.sibling);
                        this->keys.push_back(children_split_result.key);
                    }
                    else
                    {
                        this->children.insert(this->children.begin() + upper_index + 1, children_split_result.sibling);
                        this->keys.insert(this->keys.begin() + upper_index, children_split_result.key);
                    }

                    result.split = this->children.size() == max_size + 1;
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
    split_result result;
    btree_internal_node* sibling = new btree_internal_node();
    for (size_t i = min_size; i < this->children.size(); i++)
    {
        sibling->children.push_back(this->children[i]);
    }
    for (size_t i = min_size; i < this->keys.size(); i++)
    {
        sibling->keys.push_back(this->keys[i]);
    }
    result.sibling = sibling;
    result.key = this->keys[min_size - 1];
    this->keys.resize(min_size - 1);
    this->children.resize(min_size);
    return result;
}

bool btree_internal_node::select(int key, int* result)
{
    for (size_t upper_index = 0; upper_index <= this->keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->keys.size()) || (key < this->keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            return this->children[upper_index]->select(key, result);
        }
    }

    return false;
}

remove_result btree_internal_node::remove(int key)
{
    remove_result result;
    result.succeed = false;
    for (size_t upper_index = 0; upper_index <= this->keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->keys.size()) || (key < this->keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            return this->children[upper_index]->remove(key);
        }
    }

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
            this->m_root = new btree_internal_node(split_result.key, this->m_root, split_result.sibling);
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

bool btree_impl::select(int key, int* result)
{
    if (this->m_root == nullptr)
    {
        return false;
    }
    else
    {
        return this->m_root->select(key, result);
    }
}

bool btree_impl::remove(int key)
{
    if (this->m_root == nullptr)
    {
        return false;
    }
    else
    {
        remove_result result = this->m_root->remove(key);
        if (result.succeed)
        {
            // TODO: Consider merging cases
            return true;
        }
        else
        {
            return false;
        }
    }
}
