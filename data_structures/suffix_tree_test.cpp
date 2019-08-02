#include "suffix_tree.h"
#include "suffix_tree_test.h"

int suffix_tree_test()
{
    unsigned int length = 100000;
    suffix_tree s;
    for (unsigned int i = 0; i < length; i++)
    {
        s.append(rand() % 26 + 'A');
    }
    return 0;
}