#include <iostream>
#include "fenwick_tree.h"

using namespace std;

void fenwick_tree_test()
{
    fenwick_tree tree(10);
    for (int i = 0; i < 10; i++)
    {
        tree.update(i, i);
    }
    for (int i = 1; i < 10; i++)
    {
        cout << tree.running_sum(i) << endl;
    }
}