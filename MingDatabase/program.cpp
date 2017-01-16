#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    for (int i = 0; i < 8; i++)
    {
        cout << index.insert(i + 1, 8 - i) << endl;
    }

    cout << "------------" << endl;
    int result;
    for (int i = 0; i < 10; i++)
    {
        cout << index.select(i, &result) << endl;
        cout << result << endl;
    }

    return 0;
}