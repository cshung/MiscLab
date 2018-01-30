#include "suffix_tree.h"
#include <cassert>
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
    void search(int start, int end);
    bool add(int start, int end);
    char first(node* node);
    int length(node* node);
    string m_s;

    node* m_root;
    node* m_node_cursor;
    int m_edge_cursor;

    node* m_next_node_cursor;
    int m_next_text_cursor;


    int m_end;

    node* m_last_internal_node;
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
    this->m_root = new node();
    unsigned int start = 0;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_node_cursor = this->m_root;
        this->m_edge_cursor = 0;
        this->m_end = end;
        for (; start < end; start++)
        {
            if (this->add(start, end))
            {
                break;
            }
        }
    }
}

int suffix_tree_impl::length(node* node)
{
    if (node == this->m_root)
    {
        return 0;
    }
    else if (node->m_first_child == nullptr)
    {
        return this->m_end - node->m_begin;
    }
    else
    {
        return node->m_end - node->m_begin;
    }
}

char suffix_tree_impl::first(node* node)
{
    return this->m_s[node->m_begin];
}

void suffix_tree_impl::search(int start, int end)
{
    int text_cursor = start;
    while (text_cursor < end)
    {
        int node_length = length(this->m_node_cursor);
        if (this->m_edge_cursor == node_length)
        {
            if (this->m_node_cursor->m_suffix_link != nullptr)
            {
                this->m_next_node_cursor = this->m_node_cursor->m_suffix_link;
                this->m_next_text_cursor = text_cursor - 1;
            }

            char next_char = this->m_s[text_cursor];
            node* child_cursor = this->m_node_cursor->m_first_child;
            while (true)
            {
                assert(child_cursor != nullptr);
                if (this->first(child_cursor) == next_char)
                {
                    this->m_node_cursor = child_cursor;
                    this->m_edge_cursor = 0;
                    break;
                }
                else
                {
                    child_cursor = child_cursor->m_sibling;
                }
            }
        }
        else
        {
            int text_move = end - text_cursor;
            int edge_move = node_length - this->m_edge_cursor;
            int move = text_move > edge_move ? edge_move : text_move;
            this->m_edge_cursor += move;
            text_cursor += move;
        }
    }
}

bool suffix_tree_impl::add(int start, int end)
{
    bool no_op_applied = false;

    this->m_node_cursor = this->m_next_node_cursor;
    start = this->m_next_text_cursor;
    this->search(start, end - 1);

    char next_text_char = this->m_s[end - 1];
    node* search_end = nullptr;
    node* new_internal_node = nullptr;
    if (this->m_edge_cursor == length(this->m_node_cursor))
    {
        if (this->m_node_cursor != this->m_root && this->m_node_cursor->m_first_child == nullptr)
        {
        }
        else
        {
            node* search = this->m_node_cursor->m_first_child;
            bool found = false;
            while (search != nullptr)
            {
                if (first(search) == next_text_char)
                {
                    found = true;
                    break;
                }
                else
                {
                    search = search->m_sibling;
                }
            }
            if (found)
            {
                no_op_applied = true;
            }
            else
            {
                node* new_leaf = new node();
                new_leaf->m_begin = end - 1;
                new_leaf->m_parent = m_node_cursor;
                new_leaf->m_sibling = m_node_cursor->m_first_child;
                this->m_node_cursor->m_first_child = new_leaf;
            }
        }
        search_end = this->m_node_cursor;
    }
    else
    {
        char next_tree_char = this->m_s[this->m_node_cursor->m_begin + this->m_edge_cursor];
        if (next_text_char == next_tree_char)
        {
            no_op_applied = true;
        }
        else
        {
            node* new_node = new node();
            node* new_leaf = new node();
            new_leaf->m_begin = end - 1;
            new_node->m_begin = this->m_node_cursor->m_begin;
            new_node->m_end = this->m_node_cursor->m_begin + this->m_edge_cursor;

            new_node->m_parent = this->m_node_cursor->m_parent;
            new_leaf->m_parent = new_node;
            this->m_node_cursor->m_parent = new_node;

            new_node->m_sibling = new_node->m_parent->m_first_child;
            new_node->m_parent->m_first_child = new_node;
            node* search = new_node;
            while (search != nullptr)
            {
                if (search->m_sibling == this->m_node_cursor)
                {
                    search->m_sibling = search->m_sibling->m_sibling;
                    break;
                }
            }
            new_internal_node = search_end = new_node;
        }
    }

    if (this->m_last_internal_node != nullptr)
    {
        assert(this->m_last_internal_node->m_suffix_link == nullptr);
        assert(search_end != nullptr);
        this->m_last_internal_node->m_suffix_link = search_end;
        this->m_last_internal_node = nullptr;
    }

    if (new_internal_node != nullptr)
    {
        this->m_last_internal_node = new_internal_node;
    }

    return no_op_applied;
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
