#include "suffix_tree.h"
using namespace std;

class node
{
public:
    node();
    ~node();
    int m_begin;
    int m_end;
    node* m_parent;
    node* m_first_child;
    node* m_sibling;
    node* m_suffix_link;
};

class suffix_tree_impl
{
public:
    suffix_tree_impl(string s);
    ~suffix_tree_impl();
private:
    bool add(int start, int end);
    string m_s;
    node* m_root;
};

node::node() : m_begin(0), m_end(0), m_parent(nullptr), m_first_child(nullptr), m_sibling(nullptr), m_suffix_link(nullptr)
{

}

node::~node()
{
    if (this->m_first_child != nullptr)
    {
        delete this->m_first_child;
    }
    if (this->m_sibling != nullptr)
    {
        delete this->m_sibling;
    }
}

suffix_tree_impl::suffix_tree_impl(string s) : m_s(s)
{
    m_root = new node();
    node* nextStart = nullptr;
    int nextDepth = 0;
    int currentEnd = 0;
    unsigned int start = 0;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        nextStart = this->m_root;
        nextDepth = 0;
        currentEnd = end;
        for (; start < end; start++)
        {
            if (add(start, end))
            {
                break;
            }
        }
    }
}

bool suffix_tree_impl::add(int start, int end)
{
    return true;
}

suffix_tree_impl::~suffix_tree_impl()
{
    delete this->m_root;
}

suffix_tree::suffix_tree(string s) : m_impl(new suffix_tree_impl(s))
{

}

suffix_tree::~suffix_tree()
{
    delete this->m_impl;
}
