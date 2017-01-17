#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    index.insert(1, 2);
    index.insert(2, 2);
    index.insert(3, 2);
    index.insert(4, 2);
    index.insert(5, 2);

    index.print();

    // Here is a bug case - even underflow does not happen, we do need to handle the internal node changes
    index.remove(3);

    cout << "After remove" << endl;
    index.print();

    return 0;
}