#include "suffix_tree.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

class suffix_tree_node
{
public:
    suffix_tree_node();
    ~suffix_tree_node();
    int m_id;
    int m_begin;
    int m_end;
    suffix_tree_node* m_parent;
    suffix_tree_node* m_first_child;
    suffix_tree_node* m_sibling;
    suffix_tree_node* m_suffix_link;
private:
    static int s_id;
};

int suffix_tree_node::s_id = 0;

class suffix_tree_impl
{
public:
    suffix_tree_impl();
    void append(char c);
    void show();
    suffix_tree_node* get_root();
    suffix_tree_node* get_child(suffix_tree_node* n);
    suffix_tree_node* get_sibling(suffix_tree_node* n);
    void get_edge(suffix_tree_node* n, int* pBegin, int* pEnd);
    ~suffix_tree_impl();
private:
    char first_char(suffix_tree_node* node);
    int length(suffix_tree_node* node);
    void phase();
    bool extension(suffix_tree_node*& next_node_cursor, int& next_text_cursor);
    void search(suffix_tree_node*& next_node_cursor, int& next_text_cursor, suffix_tree_node*& node_cursor, int& edge_cursor);
    void show(suffix_tree_node* node, int indent);
    vector<char> m_s;
    suffix_tree_node* m_root;

    suffix_tree_node* m_last_internal_node;
    int m_start;
};

suffix_tree_node::suffix_tree_node() : m_id(++s_id), m_begin(0), m_end(0), m_parent(nullptr), m_first_child(nullptr), m_sibling(nullptr), m_suffix_link(nullptr)
{

}

suffix_tree_node::~suffix_tree_node()
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

suffix_tree_impl::suffix_tree_impl()
{
    this->m_root = new suffix_tree_node();
    this->m_last_internal_node = nullptr;
    this->m_start = 0;
}

void suffix_tree_impl::append(char c)
{
    this->m_s.push_back(c);    
    this->phase();
}

void suffix_tree_impl::phase()
{
    suffix_tree_node* next_node_cursor = this->m_root;
    int next_text_cursor = this->m_start;

    cout << "1. " << this->m_s.size() << endl;
    for (; this->m_start < this->m_s.size(); this->m_start++)
    {
        cout << "2. " << this->m_start << "," << this->m_s.size() << endl;        
        if (this->extension(next_node_cursor, next_text_cursor))
        {
            break;
        }
    }
}

bool suffix_tree_impl::extension(suffix_tree_node*& next_node_cursor, int& next_text_cursor)
{
    suffix_tree_node* node_cursor = next_node_cursor;
    int edge_cursor = length(node_cursor);
    bool no_op_applied = false;

    this->search(next_node_cursor, next_text_cursor, node_cursor, edge_cursor);

    char next_text_char = this->m_s[this->m_s.size() - 1];
    suffix_tree_node* search_end = nullptr;
    suffix_tree_node* new_internal_node = nullptr;
    if (edge_cursor == length(node_cursor))
    {
        if (node_cursor != this->m_root && node_cursor->m_first_child == nullptr)
        {
        }
        else
        {
            suffix_tree_node* search = node_cursor->m_first_child;
            bool found = false;
            cout << "8. " << node_cursor->m_id << endl;
            // cout << "7. Search: ";
            while (search != nullptr)
            {
                // cout << first_char(search);
                if (first_char(search) == next_text_char)
                {
                    found = true;
                    break;
                }
                // else
                {
                    search = search->m_sibling;
                }
            }
            // cout << endl;
            if (found)
            {
                cout << "4. NoOp " << next_text_char << endl;
                no_op_applied = true;
            }
            else
            {
                cout << "6. Split " << next_text_char << endl;
                suffix_tree_node* new_leaf = new suffix_tree_node();
                new_leaf->m_begin = this->m_s.size() - 1;
                new_leaf->m_parent = node_cursor;
                new_leaf->m_sibling = node_cursor->m_first_child;
                node_cursor->m_first_child = new_leaf;
            }
        }
        search_end = node_cursor;
    }
    else
    {
        char next_tree_char = this->m_s[node_cursor->m_begin + edge_cursor];
        if (next_text_char == next_tree_char)
        {
            cout << "5. NoOp" << endl;
            no_op_applied = true;
        }
        else
        {
            suffix_tree_node* new_node = new suffix_tree_node();
            suffix_tree_node* new_leaf = new suffix_tree_node();
            new_leaf->m_begin = this->m_s.size() - 1;
            new_node->m_begin = node_cursor->m_begin;
            new_node->m_end = node_cursor->m_begin + edge_cursor;
            node_cursor->m_begin = node_cursor->m_begin + edge_cursor;

            new_node->m_parent = node_cursor->m_parent;
            new_leaf->m_parent = new_node;
            node_cursor->m_parent = new_node;

            new_node->m_sibling = new_node->m_parent->m_first_child;
            new_node->m_parent->m_first_child = new_node;

            suffix_tree_node* search = new_node;
            while (search != nullptr)
            {
                if (search->m_sibling == node_cursor)
                {
                    search->m_sibling = search->m_sibling->m_sibling;
                    break;
                }
                search = search->m_sibling;
            }

            new_node->m_first_child = new_leaf;
            new_leaf->m_sibling = node_cursor;
            node_cursor->m_sibling = nullptr;

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

    if (no_op_applied)
    {
        cout << "3. Break" << endl;
        return true;
    }

    return false;
}

void suffix_tree_impl::search(suffix_tree_node*& next_node_cursor, int& next_text_cursor, suffix_tree_node*& node_cursor, int& edge_cursor)
{
    int text_cursor = next_text_cursor;
    next_node_cursor = this->m_root;
    next_text_cursor = text_cursor + 1;
    while (text_cursor < this->m_s.size() - 1)
    {
        cout << "10. " << edge_cursor << endl;
        cout << "13. " << node_cursor->m_id << endl;
        int node_length = length(node_cursor);
        if (edge_cursor == node_length)
        {
            if (node_cursor->m_suffix_link != nullptr)
            {
                next_node_cursor = node_cursor->m_suffix_link;
                next_text_cursor = text_cursor;
                cout << "9. " << next_text_cursor << endl;
            }

            char next_char = this->m_s[text_cursor];
            suffix_tree_node* child_cursor = node_cursor->m_first_child;
            cout << "14. " << text_cursor << next_char << endl;
            while (true)
            {
                assert(child_cursor != nullptr);
                if (this->first_char(child_cursor) == next_char)
                {
                    node_cursor = child_cursor;
                    edge_cursor = 0;
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
            int text_move = this->m_s.size() - 1 - text_cursor;
            int edge_move = node_length - edge_cursor;
            int move = text_move > edge_move ? edge_move : text_move;
            cout << "12. " << move << "," << text_move << "," << edge_move << endl;
            edge_cursor += move;
            text_cursor += move;
        }
    }
    cout << "11. " << edge_cursor << endl;
}

void suffix_tree_impl::show()
{
    this->show(this->m_root, 0);
}

suffix_tree_node* suffix_tree_impl::get_root()
{
    return this->m_root;
}

suffix_tree_node* suffix_tree_impl::get_child(suffix_tree_node* n)
{
    return n->m_first_child;
}

suffix_tree_node* suffix_tree_impl::get_sibling(suffix_tree_node* n)
{
    return n->m_sibling;
}

void suffix_tree_impl::get_edge(suffix_tree_node* n, int* pBegin, int* pEnd)
{
    int length = this->length(n);
    *pBegin = n->m_begin;
    *pEnd = n->m_begin + length;
}

void suffix_tree_impl::show(suffix_tree_node* n, int indent)
{
    for (int i = 0; i< indent; i++)
    {
        cout << " ";
    }
    cout << "'";
    for (int i = 0; i < this->length(n); i++)
    {
        cout << this->m_s[n->m_begin + i];
    }
    cout << "'";
    cout << endl;
    
    suffix_tree_node* child = n->m_first_child;
    while (child != nullptr)
    {
        this->show(child, indent + 1);
        child = child->m_sibling;
    }
}

int suffix_tree_impl::length(suffix_tree_node* node)
{
    if (node == this->m_root)
    {
        return 0;
    }
    else if (node->m_first_child == nullptr)
    {
        return this->m_s.size() - node->m_begin;
    }
    else
    {
        return node->m_end - node->m_begin;
    }
}

char suffix_tree_impl::first_char(suffix_tree_node* node)
{
    return this->m_s[node->m_begin];
}

suffix_tree_impl::~suffix_tree_impl()
{
    delete this->m_root;
}

suffix_tree::suffix_tree() : m_impl(new suffix_tree_impl())
{
}

void suffix_tree::append(char c)
{
    this->m_impl->append(c);
}

void suffix_tree::show()
{
    this->m_impl->show();
}

suffix_tree::~suffix_tree()
{
    delete this->m_impl;
}

suffix_tree_node* suffix_tree::get_root()
{
    return this->m_impl->get_root();
}

suffix_tree_node* suffix_tree::get_child(suffix_tree_node* n)
{
    return this->m_impl->get_child(n);
}

suffix_tree_node* suffix_tree::get_sibling(suffix_tree_node* n)
{
    return this->m_impl->get_sibling(n);
}

void suffix_tree::get_edge(suffix_tree_node* n, int* pBegin, int* pEnd)
{
    this->m_impl->get_edge(n, pBegin, pEnd);
}
