#pragma once

class splay_tree
{
public:
    splay_tree();
    ~splay_tree();

    bool try_insert(int key, int value);
    bool try_get(int key, int* value);
    bool try_delete(int key);
private:

    class splay_tree_node
    {
    public:
        splay_tree_node(int key, int value);
        splay_tree_node* m_left;
        splay_tree_node* m_right;

        int m_key;
        int m_value;
    };

    splay_tree::splay_tree_node* splay(splay_tree::splay_tree_node* root, int key);

    void delete_node_recursively(splay_tree::splay_tree_node* node);

    splay_tree_node* m_root;
};
