#include "btree.h"

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

// TODO: For ease to debug, start with a simple 2-3 tree
const int max_size = 3;
const int min_size = 2;

class btree_node;

struct insert_result
{
    bool succeed;
    bool overflow;
};

struct split_result
{
    btree_node* sibling;
    int key;
};

struct remove_result
{
    bool succeed;
    bool underflow;
    int replacement_key;
};

class btree_node
{
public:
    btree_node();
    virtual ~btree_node();
    virtual insert_result insert(int key, int value) = 0;
    virtual split_result split() = 0;
    virtual bool select(int key, int* result) const = 0;
    virtual remove_result remove(int key) = 0;
    virtual bool can_borrow() = 0;
    virtual bool can_accept() = 0;
    virtual void pull(btree_node* accepter, int* key) = 0;
    virtual void push(btree_node* accepter, int* key) = 0;
    virtual void merge_right(int key, btree_node* other) = 0;
    virtual btree_node* get_replacement_root() = 0;

    virtual void print(int indent) const = 0;
    virtual void verify(btree_node* root, int min, int max) const = 0;
};

class btree_internal_node : public btree_node
{
public:
    btree_internal_node(int key, btree_node* left, btree_node* right);
    ~btree_internal_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
    virtual bool select(int key, int* result) const;
    virtual remove_result remove(int key);
    virtual bool can_borrow();
    virtual bool can_accept();
    virtual void pull(btree_node* accepter, int* key);
    virtual void push(btree_node* accepter, int* key);
    virtual void merge_right(int key, btree_node* other);
    virtual btree_node* get_replacement_root();

    virtual void print(int indent) const;
    virtual void verify(btree_node* root, int min, int max) const;
private:
    btree_internal_node();
    vector<int> internal_keys;
    vector<btree_node*> children;
};

class btree_leaf_node : public btree_node
{
public:
    btree_leaf_node();
    virtual ~btree_leaf_node();
    virtual insert_result insert(int key, int value);
    virtual split_result split();
    virtual bool select(int key, int* result) const;
    virtual remove_result remove(int key);
    virtual bool can_borrow();
    virtual bool can_accept();
    virtual void pull(btree_node* accepter, int* key);
    virtual void push(btree_node* accepter, int* key);
    virtual void merge_right(int key, btree_node* other);
    virtual btree_node* get_replacement_root();

    virtual void print(int indent) const;
    virtual void verify(btree_node* root, int min, int max) const;
private:
    vector<int> external_keys;
    vector<int> values;
    btree_leaf_node* right_sibling;
};

class btree_impl
{
public:
    ~btree_impl();
    bool insert(int key, int value);
    bool select(int key, int* result) const;
    bool remove(int key);

    void print() const;
    void verify() const;
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

bool btree::select(int key, int* result) const
{
    return this->m_impl->select(key, result);
}

bool btree::remove(int key)
{
    return this->m_impl->remove(key);
}

void btree::print() const
{
    this->m_impl->print();
}

void btree::verify() const
{
    this->m_impl->verify();
}

btree_node::btree_node()
{
}

btree_node::~btree_node()
{
}

btree_leaf_node::btree_leaf_node()
{
    this->right_sibling = nullptr;
}

btree_leaf_node::~btree_leaf_node()
{
    this->right_sibling = nullptr;
}

insert_result btree_leaf_node::insert(int key, int value)
{
    insert_result result;
    for (size_t upper_index = 0; upper_index <= this->external_keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->external_keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->external_keys.size()) || (key < this->external_keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            if (lower_index != -1 && key == this->external_keys[lower_index])
            {
                result.succeed = false;
            }
            else
            {
                this->external_keys.insert(this->external_keys.begin() + upper_index, key);
                this->values.insert(this->values.begin() + upper_index, value);
                result.succeed = true;
                result.overflow = (this->external_keys.size() == (max_size + 1));
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
    sibling->right_sibling = this->right_sibling;
    for (size_t i = min_size; i < this->external_keys.size(); i++)
    {
        sibling->external_keys.push_back(this->external_keys[i]);
        sibling->values.push_back(this->values[i]);
    }
    this->external_keys.resize(min_size);
    this->values.resize(min_size);

    result.sibling = sibling;
    result.key = sibling->external_keys[0];

    this->right_sibling = sibling;
    return result;
}

bool btree_leaf_node::select(int key, int* result) const
{
    for (size_t i = 0; i < this->external_keys.size(); i++)
    {
        if (this->external_keys[i] == key)
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
    for (size_t i = 0; i < this->external_keys.size(); i++)
    {
        if (this->external_keys[i] == key)
        {
            result.succeed = true;
            this->external_keys.erase(this->external_keys.begin() + i);
            this->values.erase(this->values.begin() + i);
            break;
        }
    }

    result.replacement_key = this->external_keys[0];
    result.underflow = this->external_keys.size() < min_size;
    return result;
}

bool btree_leaf_node::can_borrow()
{
    return this->external_keys.size() > min_size;
}

bool btree_leaf_node::can_accept()
{
    return this->external_keys.size() < max_size;
}

void btree_leaf_node::pull(btree_node* accepter, int* key)
{
    btree_leaf_node* accepter_node = (btree_leaf_node*)accepter;
    accepter_node->external_keys.push_back(this->external_keys[0]);
    accepter_node->values.push_back(this->values[0]);
    this->external_keys.erase(this->external_keys.begin());
    this->values.erase(this->values.begin());
    *key = this->external_keys[0];
}

void btree_leaf_node::push(btree_node* accepter, int* key)
{
    btree_leaf_node* accepter_node = (btree_leaf_node*)accepter;
    int my_size = this->external_keys.size();
    *key = this->external_keys[my_size - 1];
    accepter_node->external_keys.insert(accepter_node->external_keys.begin(), *key);
    accepter_node->values.insert(accepter_node->values.begin(), this->values[my_size - 1]);
    this->external_keys.resize(my_size - 1);
    this->values.resize(my_size - 1);
}

void btree_leaf_node::merge_right(int key, btree_node* other)
{
    btree_leaf_node* other_node = (btree_leaf_node*)other;
    for (size_t i = 0; i < other_node->external_keys.size(); i++)
    {
        this->external_keys.push_back(other_node->external_keys[i]);
    }
    for (size_t i = 0; i < other_node->values.size(); i++)
    {
        this->values.push_back(other_node->values[i]);
    }

    other_node->external_keys.clear();
    other_node->values.clear();
}

btree_node* btree_leaf_node::get_replacement_root()
{
    if (this->external_keys.size() == 0)
    {
        delete this;
        return nullptr;
    }
    else
    {
        return this;
    }
}

void btree_leaf_node::print(int indent) const
{
    for (size_t i = 0; i < this->external_keys.size(); i++)
    {
        for (int j = 0; j < indent; j++)
        {
            cout << " ";
        }
        cout << this->external_keys[i] << "->" << this->values[i] << endl;
    }
}

void btree_leaf_node::verify(btree_node* root, int min, int max) const
{
    for (size_t i = 0; i < this->external_keys.size(); i++)
    {
        if (this->external_keys[i] < min || this->external_keys[i] >= max)
        {
            assert(false);
        }
    }
    if (this->external_keys.size() > max_size || this->external_keys.size() < min_size)
    {
        if (this != root)
        {
            assert(false);
        }
    }
}

btree_internal_node::btree_internal_node(int key, btree_node* left, btree_node* right)
{
    this->internal_keys.push_back(key);
    this->children.push_back(left);
    this->children.push_back(right);
}

btree_internal_node::btree_internal_node()
{

}

btree_internal_node::~btree_internal_node()
{
    for (size_t i = 0; i < this->children.size(); i++)
    {
        delete this->children[i];
    }
}

insert_result btree_internal_node::insert(int key, int value)
{
    insert_result result;
    for (size_t upper_index = 0; upper_index <= this->internal_keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->internal_keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->internal_keys.size()) || (key < this->internal_keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            insert_result children_insert_result = this->children[upper_index]->insert(key, value);
            if (children_insert_result.succeed)
            {
                result.succeed = true;
                if (children_insert_result.overflow)
                {
                    bool overflow_solved = false;
                    if (upper_index > 0)
                    {
                        if (this->children[upper_index - 1]->can_accept())
                        {
                            this->children[upper_index]->pull(this->children[upper_index - 1], &(this->internal_keys[upper_index - 1]));

                            overflow_solved = true;
                        }
                    }
                    if (!overflow_solved && upper_index < this->children.size() - 1)
                    {
                        if (this->children[upper_index + 1]->can_accept())
                        {
                            this->children[upper_index]->push(this->children[upper_index + 1], &(this->internal_keys[upper_index]));
                            overflow_solved = true;
                        }
                    }
                    if (!overflow_solved)
                    {
                        split_result children_split_result = this->children[upper_index]->split();
                        if (upper_index == this->internal_keys.size())
                        {
                            this->children.push_back(children_split_result.sibling);
                            this->internal_keys.push_back(children_split_result.key);
                        }
                        else
                        {
                            this->children.insert(this->children.begin() + upper_index + 1, children_split_result.sibling);
                            this->internal_keys.insert(this->internal_keys.begin() + upper_index, children_split_result.key);
                        }
                    }

                    result.overflow = this->children.size() == max_size + 1;
                }
                else
                {
                    result.overflow = false;
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
    for (size_t i = min_size; i < this->internal_keys.size(); i++)
    {
        sibling->internal_keys.push_back(this->internal_keys[i]);
    }
    result.sibling = sibling;
    result.key = this->internal_keys[min_size - 1];
    this->internal_keys.resize(min_size - 1);
    this->children.resize(min_size);
    return result;
}

bool btree_internal_node::select(int key, int* result) const
{
    for (size_t upper_index = 0; upper_index <= this->internal_keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->internal_keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->internal_keys.size()) || (key < this->internal_keys[upper_index]);
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
    for (size_t upper_index = 0; upper_index <= this->internal_keys.size(); upper_index++)
    {
        int lower_index = upper_index - 1;
        bool lower_is_good = (lower_index == -1) || (this->internal_keys[lower_index] <= key);
        bool upper_is_good = (upper_index == this->internal_keys.size()) || (key < this->internal_keys[upper_index]);
        if (lower_is_good && upper_is_good)
        {
            remove_result child_remove_result = this->children[upper_index]->remove(key);
            if (child_remove_result.succeed)
            {
                result.replacement_key = child_remove_result.replacement_key;
                if (lower_index != -1)
                {
                    if (this->internal_keys[lower_index] == key)
                    {
                        this->internal_keys[lower_index] = result.replacement_key;
                    }
                }

                result.succeed = true;
                if (child_remove_result.underflow)
                {
                    bool underflow_solved = false;
                    if (upper_index > 0)
                    {
                        if (this->children[upper_index - 1]->can_borrow())
                        {
                            this->children[upper_index - 1]->push(this->children[upper_index], &(this->internal_keys[upper_index - 1]));
                            underflow_solved = true;
                        }
                    }
                    if (!underflow_solved && upper_index < this->children.size() - 1)
                    {
                        if (this->children[upper_index + 1]->can_borrow())
                        {
                            this->children[upper_index + 1]->pull(this->children[upper_index], &(this->internal_keys[upper_index]));
                            underflow_solved = true;
                        }
                    }
                    if (!underflow_solved && upper_index > 0)
                    {
                        this->children[upper_index - 1]->merge_right(this->internal_keys[upper_index - 1], this->children[upper_index]);
                        delete this->children[upper_index];
                        this->internal_keys.erase(this->internal_keys.begin() + (upper_index - 1));
                        this->children.erase(this->children.begin() + upper_index);
                        underflow_solved = true;
                    }
                    if (!underflow_solved && upper_index < this->children.size() - 1)
                    {
                        this->children[upper_index]->merge_right(this->internal_keys[upper_index], this->children[upper_index + 1]);
                        delete this->children[upper_index + 1];
                        this->internal_keys.erase(this->internal_keys.begin() + upper_index);
                        this->children.erase(this->children.begin() + (upper_index + 1));
                        underflow_solved = true;
                    }
                }

                result.underflow = this->children.size() < min_size;
            }

            break;
        }
    }

    return result;
}

bool btree_internal_node::can_borrow()
{
    return this->children.size() > min_size;
}

bool btree_internal_node::can_accept()
{
    return this->children.size() < max_size;
}

void btree_internal_node::pull(btree_node* accepter, int* key)
{
    btree_internal_node* accepter_node = (btree_internal_node*)accepter;
    accepter_node->children.push_back(this->children[0]);
    accepter_node->internal_keys.push_back(*key);
    *key = this->internal_keys[0];
    this->internal_keys.erase(this->internal_keys.begin());
    this->children.erase(this->children.begin());
}

void btree_internal_node::push(btree_node* accepter, int* key)
{
    btree_internal_node* accepter_node = (btree_internal_node*)accepter;
    accepter_node->children.insert(accepter_node->children.begin(), this->children[this->children.size() - 1]);
    accepter_node->internal_keys.insert(accepter_node->internal_keys.begin(), *key);
    *key = this->internal_keys[this->internal_keys.size() - 1];
    this->internal_keys.resize(this->internal_keys.size() - 1);
    this->children.resize(this->children.size() - 1);
}

void btree_internal_node::merge_right(int key, btree_node* other)
{
    btree_internal_node* other_node = (btree_internal_node*)other;
    this->internal_keys.push_back(key);
    for (size_t i = 0; i < other_node->internal_keys.size(); i++)
    {
        this->internal_keys.push_back(other_node->internal_keys[i]);
    }
    for (size_t i = 0; i < other_node->children.size(); i++)
    {
        this->children.push_back(other_node->children[i]);
    }

    other_node->internal_keys.clear();
    other_node->children.clear();
}

btree_node* btree_internal_node::get_replacement_root()
{
    if (this->children.size() == 1)
    {
        btree_node* result = this->children[0];
        this->children.clear();
        delete this;
        return result;
    }
    else
    {
        return this;
    }
}

void btree_internal_node::print(int indent) const
{
    children[0]->print(indent + 2);
    for (size_t i = 0; i < this->internal_keys.size(); i++)
    {
        for (int j = 0; j < indent; j++)
        {
            cout << " ";
        }
        cout << internal_keys[i] << endl;
        children[i + 1]->print(indent + 2);
    }
}

void btree_internal_node::verify(btree_node* root, int min, int max) const
{
    for (size_t i = 0; i < this->internal_keys.size(); i++)
    {
        if (this->internal_keys[i] < min || this->internal_keys[i] >= max)
        {
            assert(false);
        }
    }
    this->children[0]->verify(root, min, this->internal_keys[0]);
    for (size_t i = 1; i < this->internal_keys.size() - 1; i++)
    {
        this->children[i]->verify(root, this->internal_keys[i - 1], this->internal_keys[i]);
    }
    this->children[this->internal_keys.size()]->verify(root, this->internal_keys[this->internal_keys.size() - 1], max);

    if (this->children.size() > max_size || this->children.size() < min_size)
    {
        if (this != root)
        {
            assert(false);
        }
    }
}

btree_impl::~btree_impl()
{
    if (this->m_root != nullptr)
    {
        delete this->m_root;
    }
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
        if (result.overflow)
        {
            split_result split_result = this->m_root->split();
            this->m_root = new btree_internal_node(split_result.key, this->m_root, split_result.sibling);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool btree_impl::select(int key, int* result) const
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
            if (result.underflow)
            {
                this->m_root = this->m_root->get_replacement_root();
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}

void btree_impl::print() const
{
    if (this->m_root != nullptr)
    {
        this->m_root->print(0);
    }
}

void btree_impl::verify() const
{
    if (this->m_root != nullptr)
    {
        int min = 1 << 31;
        int max = ~min;
        this->m_root->verify(this->m_root, min, max);
    }
}