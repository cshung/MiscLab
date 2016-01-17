#include <iostream>
#include "splay_tree.h"
using namespace std;

int main(int argc, char** argv)
{
    splay_tree tree;
    int value;

    tree.try_insert(1, 10086);
    tree.try_insert(2, 10086);
    tree.try_insert(3, 10086);
    tree.try_insert(4, 10086);
    tree.try_insert(5, 10086);
    tree.try_insert(6, 10086);
    tree.try_insert(7, 10086);
    tree.try_get(1, &value);
    tree.try_get(2, &value);
    tree.try_get(3, &value);

    tree.try_delete(3);
    cout << value << endl;
    return 0;
}