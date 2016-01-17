#include "splay_tree.h"
#include <cassert>
#include <stack>

using namespace std;

splay_tree::splay_tree() : m_root(nullptr)
{
}

splay_tree::~splay_tree()
{
    this->delete_node_recursively(this->m_root);
}

void splay_tree::delete_node_recursively(splay_tree_node* node)
{
    if (node != nullptr)
    {
        this->delete_node_recursively(node->m_left);
        this->delete_node_recursively(node->m_right);
        delete node;
    }
}

splay_tree::splay_tree_node::splay_tree_node(int key, int value) : m_left(nullptr), m_right(nullptr), m_key(key), m_value(value)
{
}

bool splay_tree::try_get(int key, int* value)
{
    this->m_root = this->splay(this->m_root, key);
    if (this->m_root == nullptr || this->m_root->m_key != key)
    {
        return false;
    }
    else
    {
        *value = this->m_root->m_value;
        return true;
    }
}

bool splay_tree::try_insert(int key, int value)
{
    this->m_root = this->splay(this->m_root, key);

    if (this->m_root == nullptr)
    {
        this->m_root = new splay_tree::splay_tree_node(key, value);

    }
    else if (this->m_root->m_key == key)
    {
        // Key already exists
        return false;
    }
    else
    {
        splay_tree::splay_tree_node* new_node = new splay_tree::splay_tree_node(key, value);
        if (key < this->m_root->m_key)
        {
            splay_tree::splay_tree_node* root_left = this->m_root->m_left;
            this->m_root->m_left = new_node;
            if (root_left != nullptr)
            {
                if (root_left->m_key < new_node->m_key)
                {
                    new_node->m_left = root_left;
                }
                else
                {
                    assert(new_node->m_key < root_left->m_key);
                    new_node->m_right = root_left;
                }
            }
        }
        else
        {
            assert(this->m_root->m_key < key);
            splay_tree::splay_tree_node* root_right = this->m_root->m_right;
            this->m_root->m_right = new_node;
            if (root_right != nullptr)
            {
                if (root_right->m_key < new_node->m_key)
                {
                    new_node->m_left = root_right;
                }
                else
                {
                    assert(new_node->m_key < root_right->m_key);
                    new_node->m_right = root_right;
                }
            }
        }
    }

    return true;
}

bool splay_tree::try_delete(int key)
{
    this->m_root = this->splay(this->m_root, key);
    if (this->m_root == nullptr || this->m_root->m_key != key)
    {
        return false;
    }
    else
    {
        splay_tree::splay_tree_node* new_root = nullptr;
        if (this->m_root->m_left != nullptr)
        {
            new_root = this->splay(this->m_root->m_left, ~(1 << 31));
            assert(new_root->m_right == nullptr);
            new_root->m_right = this->m_root->m_right;
        }
        else
        {
            new_root = this->m_root->m_right;
        }

        delete this->m_root;
        this->m_root = new_root;
        return true;
    }
}

splay_tree::splay_tree_node* splay_tree::splay(splay_tree::splay_tree_node* root, int key)
{
    stack<splay_tree::splay_tree_node*> path;
    splay_tree::splay_tree_node* current = root;
    while (current != nullptr)
    {
        path.push(current);
        if (current->m_key == key)
        {
            break;
        }
        else if (current->m_key < key)
        {
            current = current->m_right;
        }
        else
        {
            current = current->m_left;
        }
    }

    if (path.size() == 0)
    {
        return nullptr;
    }

    splay_tree::splay_tree_node* p = path.top(); path.pop();
    while (path.size() > 0)
    {
        if (path.size() == 1)
        {
            // Case 1: We only have two nodes in the path, a simple rotation would suffice
            splay_tree::splay_tree_node* q = path.top(); path.pop();
            if (q->m_left == p)
            {
                //    q           p  
                //  p   c  --> a    q
                // a b             b c
                q->m_left = p->m_right;
                p->m_right = q;
            }
            else
            {
                assert(q->m_right == p);
                //   q            p
                // a   p   -->  q   c
                //    b c      a b
                q->m_right = p->m_left;
                p->m_left = q;
            }
        }
        else
        {
            splay_tree::splay_tree_node* q = path.top(); path.pop();
            splay_tree::splay_tree_node* r = path.top(); path.pop();

            if (q->m_left == p && r->m_left == q)
            {
                // Case 2: Rotate right twice
                //        r            p  
                //    q      d      a     q
                //  p    c      -->     b   r
                // a b                     c d
                r->m_left = q->m_right;
                q->m_right = r;
                q->m_left = p->m_right;
                p->m_right = q;
            }
            else if (q->m_right == p && r->m_right == q)
            {
                // Case 2: Rotate left twice
                //   r                  p
                // a    q    -->     q     d
                //    b   p        r   c
                //       c d      a b
                r->m_right = q->m_left;
                q->m_left = r;
                q->m_right = p->m_left;
                p->m_left = q;
            }
            else if (q->m_left == p && r->m_right == q)
            {
                // Case 3: Double rotate
                //    r               p
                // a     q    -->   r   q
                //     p   d       a b c d
                //    b c
                r->m_right = p->m_left;
                q->m_left = p->m_right;
                p->m_left = r;
                p->m_right = q;
            }
            else
            {
                assert(q->m_right == p && r->m_left == q);
                // Case 3: Double rotate
                //       r              p
                //   q       d -->    q   r
                // a   p             a b c d 
                //    b c
                q->m_right = p->m_left;
                r->m_left = p->m_right;
                p->m_left = q;
                p->m_right = r;
            }

            if (path.size() > 0)
            {
                splay_tree::splay_tree_node* s = path.top();
                if (s->m_left == r)
                {
                    s->m_left = p;
                }
                else
                {
                    assert(s->m_right == r);
                    s->m_right = p;
                }
            }
        }
    }

    return p;
}
