#pragma once

class suffix_tree_impl;

class suffix_tree_node;

class suffix_tree
{
public:
    suffix_tree();
    void append(char c);
    void show();
    suffix_tree_node* get_root();
    suffix_tree_node* get_child(suffix_tree_node* n);
    suffix_tree_node* get_sibling(suffix_tree_node* n);
    void get_edge(suffix_tree_node* n, int* pBegin, int* pEnd);
    ~suffix_tree();
private:
    suffix_tree_impl * m_impl;
};
