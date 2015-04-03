#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class persistent_tree
{
public:
    persistent_tree();
    ~persistent_tree();
    void insert(int x);
    void dump(unsigned int time_stamp);
private:
    class persistent_tree_node
    {
    public:
        persistent_tree_node(int data, persistent_tree_node* left, persistent_tree_node* right, int count);
        ~persistent_tree_node();

        void add_ref();
        void release();

        // Ideally these should be encapsulated - skipped for illustration
        persistent_tree_node* left;
        persistent_tree_node* right;
        unsigned int count;
        int data;
        int ref_count;
    };

    vector<persistent_tree_node*> roots;

    persistent_tree_node* recursive_insert(persistent_tree_node* current, int new_data);
    void recursive_dump(persistent_tree_node* current);
};

persistent_tree::persistent_tree()
{
}

persistent_tree::~persistent_tree()
{
    for (unsigned int i = 0; i < this->roots.size(); i++)
    {
        this->roots[i]->release();
    }
}

persistent_tree::persistent_tree_node::persistent_tree_node(int data, persistent_tree_node* left, persistent_tree_node* right, int count)
{
    this->left = left;
    this->right = right;
    this->data = data;
    this->count = 1;

    if (this->left != nullptr)
    {
        this->left->add_ref();
    }
    if (this->right != nullptr)
    {
        this->right->add_ref();
    }
}

void persistent_tree::persistent_tree_node::add_ref()
{
    this->ref_count++;
}

void persistent_tree::persistent_tree_node::release()
{
    if (--this->ref_count == 0)
    {
        delete this;
    }
}

persistent_tree::persistent_tree_node::~persistent_tree_node()
{
    if (this->left != nullptr)
    {
        this->left->release();
    }
    if (this->right != nullptr)
    {
        this->right->release();
    }
}

void persistent_tree::insert(int new_data)
{
    // Insert always work on the last time stamp
    persistent_tree_node* root = nullptr;
    if (this->roots.size() != 0)
    {
        root = this->roots[this->roots.size() - 1];
    }

    persistent_tree_node* new_root = this->recursive_insert(root, new_data);
    new_root->add_ref();
    this->roots.push_back(new_root);
}

persistent_tree::persistent_tree_node* persistent_tree::recursive_insert(persistent_tree::persistent_tree_node* current, int new_data)
{
    if (current == nullptr)
    {
        return new persistent_tree::persistent_tree_node(new_data, nullptr, nullptr, 1);
    }
    else if (current->data == new_data)
    {
        return new persistent_tree::persistent_tree_node(new_data, current->left, current->right, current->count + 1);
    }
    else if (current->data > new_data)
    {
        persistent_tree::persistent_tree_node* new_left_node = this->recursive_insert(current->left, new_data);
        return new persistent_tree::persistent_tree_node(current->data, new_left_node, current->right, current->count);
    }
    else if (current->data < new_data)
    {
        persistent_tree::persistent_tree_node* new_right_node = this->recursive_insert(current->right, new_data);
        return new persistent_tree::persistent_tree_node(current->data, current->left, new_right_node, current->count);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}

void persistent_tree::dump(unsigned int time_stamp)
{
    if (time_stamp < this->roots.size())
    {
        persistent_tree::persistent_tree_node* root = this->roots[time_stamp];
        this->recursive_dump(root);
        cout << endl;
    }
    else
    {
        assert(false);
    }
}

void persistent_tree::recursive_dump(persistent_tree::persistent_tree_node* current)
{
    if (current == nullptr)
    {
        return;
    }
    else
    {
        this->recursive_dump(current->left);
        for (unsigned int i = 0; i < current->count; i++)
        {
            // Extra comma at the end, don't care
            cout << current->data << ", ";
        }
        this->recursive_dump(current->right);
    }
}

int main(int argc, char** argv)
{
    persistent_tree tree;
    tree.insert(3);
    tree.insert(0);
    tree.insert(6);
    tree.insert(2);
    tree.insert(4);
    tree.dump(0);
    tree.dump(1);
    tree.dump(2);
    tree.dump(3);
    tree.dump(4);
    return 0;
}
