#pragma once

#include <string>

class suffix_tree_impl;

class suffix_tree
{
public:
    suffix_tree(std::string s);
    ~suffix_tree();
private:
    suffix_tree_impl * m_impl;
};