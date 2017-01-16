#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    cout << index.insert(1, 2) << endl;
    cout << index.insert(2, 2) << endl;
    cout << index.insert(3, 2) << endl;
    cout << index.insert(4, 2) << endl;
    cout << index.insert(5, 2) << endl;
    cout << index.insert(6, 2) << endl;
    cout << index.insert(7, 2) << endl;
    cout << index.insert(8, 2) << endl;
    cout << "Hello World" << endl;
    return 0;
}