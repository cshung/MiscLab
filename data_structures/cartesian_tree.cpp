#include "cartesian_tree.h"
#include <stack>
using namespace std;

cartesian_tree::cartesian_tree() : m_root(nullptr)
{
}

cartesian_tree::~cartesian_tree()
{
    if (this->m_root != nullptr)
    {
        delete this->m_root;
    }
}

cartesian_tree::cartesian_tree_node::cartesian_tree_node(int value, int priority) : m_value(value), m_priority(priority), m_left(nullptr), m_right(nullptr)
{
}

cartesian_tree::cartesian_tree_node::~cartesian_tree_node()
{
    if (this->m_left != nullptr)
    {
        delete this->m_left;
    }
    if (this->m_right != nullptr)
    {
        delete this->m_right;
    }
}

cartesian_tree::cartesian_tree_node* cartesian_tree::insert(cartesian_tree::cartesian_tree_node* current, int value, int priority)
{
    if (current == nullptr)
    {
        return new cartesian_tree_node(value, priority);
    }
    else
    {
        if (value < current->m_value)
        {
            cartesian_tree_node* result = this->insert(current->m_left, value, priority);
            if (result->m_priority < current->m_priority)
            {
                /*
                 *    d           b
                 *  b   e -->  a     d
                 * a c              c e
                 */
                cartesian_tree_node* b = result;
                cartesian_tree_node* c = result->m_right;
                cartesian_tree_node* d = current;
                b->m_right = d;
                d->m_left = c;
                return b;
            }
            else
            {
                current->m_left = result;
                return current;
            }
        }
        else
        {
            cartesian_tree_node* result = this->insert(current->m_right, value, priority);
            if (result->m_priority < current->m_priority)
            {
                /*
                 *    b           d
                 *  a   d -->  b     e
                 *     c e    a c
                 */
                cartesian_tree_node* b = current;
                cartesian_tree_node* c = result->m_left;
                cartesian_tree_node* d = result;
                b->m_right = c;
                d->m_left = b;
                return d;
            }
            else
            {
                current->m_right = result;
                return current;
            }
        }
    }

}

void cartesian_tree::insert(int value, int priority)
{
    this->m_root = this->insert(this->m_root, value, priority);
    this->validate(this->m_root);
}

void cartesian_tree::validate(cartesian_tree_node* current)
{
    if (current == nullptr)
    {
        return;
    }
    if (current->m_left != nullptr)
    {
        if (current->m_priority > current->m_left->m_priority)
        {
            throw 1;
        }
        if (current->m_value < current->m_left->m_value)
        {
            throw 2;
        }
        this->validate(current->m_left);
    }
    if (current->m_right != nullptr)
    {
        if (current->m_priority > current->m_right->m_priority)
        {
            throw 1;
        }
        if (current->m_value > current->m_right->m_value)
        {
            throw 2;
        }
        this->validate(current->m_right);
    }
}