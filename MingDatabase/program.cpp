#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    cout << index.insert(1, 2) << endl;
    cout << "Hello World" << endl;
    return 0;
}