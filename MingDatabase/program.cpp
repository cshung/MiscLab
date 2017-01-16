#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    for (int i = 0; i < 8; i++)
    {
        index.insert(i + 1, 8 - i);
    }

    cout << index.remove(1) << endl;

    return 0;
}