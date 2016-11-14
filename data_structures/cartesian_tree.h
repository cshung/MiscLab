#pragma once

class cartesian_tree
{
public:
    cartesian_tree();
    ~cartesian_tree();
    void insert(int value, int priority);
private:
    class cartesian_tree_node
    {
    public:
        cartesian_tree_node(int value, int priority);
        ~cartesian_tree_node();
        int m_value;
        int m_priority;
        cartesian_tree_node* m_left;
        cartesian_tree_node* m_right;
    };
    cartesian_tree_node* m_root;

    cartesian_tree_node* insert(cartesian_tree_node* current, int value, int priority);

    void validate(cartesian_tree_node* current);
};


